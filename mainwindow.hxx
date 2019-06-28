#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>
#include <QSettings>
#include "mcp2221_dll_um.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	enum
	{
		DEFAULT_MCP2221_USB_VID		= 0x4d8,
		DEFAULT_MCP2221_USB_PID		= 0xdd,

		OLED_I2C_ADDRESS		= 0x3c,
	};
public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	void restoreSettings(void);
	void saveSettings(void);

protected:
	void closeEvent(QCloseEvent * e) override { saveSettings(); }

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HXX
