#include "mainwindow.hxx"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
#if 1
	wchar_t version[10];
	if (!Mcp2221_GetLibraryVersion(version))
		ui->plainTextEditLog->appendPlainText("MCP2221 library version: " + QString::fromWCharArray(version, sizeof version / sizeof * version));
	else
		ui->plainTextEditLog->appendPlainText("Error retrieving MCP2221 library version!");
#endif

}

MainWindow::~MainWindow()
{
	delete ui;
}
