#include <QDebug>
#include <windows.h>

#include "mainwindow.hxx"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	int error;
	ui->setupUi(this);
	restoreSettings();

	wchar_t version[10] = { 0, };
	if (!Mcp2221_GetLibraryVersion(version))
		ui->plainTextEditLog->appendPlainText("MCP2221 library version: " + QString::fromWCharArray(version, sizeof version / sizeof * version));
	else
		ui->plainTextEditLog->appendPlainText("Error retrieving MCP2221 library version!");
	unsigned n;
	bool vidOk, pidOk;
	int vid = ui->lineEditUsbVID->text().toInt(&vidOk, 0);
	int pid = ui->lineEditUsbPID->text().toInt(&pidOk, 0);
	if (!(vidOk && pidOk))
		ui->plainTextEditLog->appendPlainText("Bad USB VID/PID value!");
	else if (!(error = Mcp2221_GetConnectedDevices(vid, pid, & n)))
		ui->plainTextEditLog->appendPlainText(QString("A total of %1 devices found").arg(n));
	else
		ui->plainTextEditLog->appendPlainText(QString("Error getting the number of connected devices, error code: %1!").arg(error));
	if (!error) do
	{
		void * h = Mcp2221_OpenByIndex(vid, pid, 0);
		if (h == INVALID_HANDLE_VALUE)
		{
			ui->plainTextEditLog->appendPlainText("Error opening MCP2221 device!");
			break;
		}
		if (Mcp2221_SetSpeed(h, 500000))
		{
			ui->plainTextEditLog->appendPlainText("Error setting i2c speed!");
			break;
		}
		unsigned char buf[] = { 0, 0x81, 0x0, 0x8d, 0x14, 0xa1, 0xc8, 0xaf, };
		if (error = Mcp2221_I2cWrite(h, sizeof buf, 0x3c, 1, buf))
		{
			ui->plainTextEditLog->appendPlainText(QString("Error writing i2c data, error code: %1!").arg(error));
			break;
		}
		for (int i = 0; i < 8; i ++)
		{
			unsigned char buf[128 + 3] = { 0x80, 0xb0 | i, 0x40, };
			for (int j = 0; j < 128; buf[j ++ + 3] = 0xff >> i);
			if (error = Mcp2221_I2cWrite(h, sizeof buf, 0x3c, 1, buf))
			{
				ui->plainTextEditLog->appendPlainText(QString("Error writing i2c data, error code: %1!").arg(error));
				break;
			}
		}
	}
	while (0);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::restoreSettings()
{
	QCoreApplication::setOrganizationName("shopov instruments");
	QCoreApplication::setApplicationName("mcp2122 console");
	QSettings s("mcp2122con.rc", QSettings::IniFormat);

	restoreGeometry(s.value("window-geometry").toByteArray());
	restoreState(s.value("window-state").toByteArray());
	ui->lineEditUsbVID->setText(s.value("vid-value", QString("0x%1").arg(DEFAULT_MCP2221_USB_VID, 4, 16, QChar('0'))).toString());
	ui->lineEditUsbPID->setText(s.value("pid-value", QString("0x%1").arg(DEFAULT_MCP2221_USB_PID, 4, 16, QChar('0'))).toString());
}

void MainWindow::saveSettings()
{
	QSettings s("mcp2122con.rc", QSettings::IniFormat);

	s.setValue("window-geometry", saveGeometry());
	s.setValue("window-state", saveState());
	s.setValue("vid-value", ui->lineEditUsbVID->text());
	s.setValue("pid-value", ui->lineEditUsbPID->text());
}
