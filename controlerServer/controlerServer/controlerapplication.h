#include <QApplication>
#include <QSharedMemory>
#include <QPointer>

//#include "common/config.h"
#include "controls/trayicon.h"
#include "controls/servermainwnd.h"
#include "Network/controlerserver.h"


class ControlerApplication : public QApplication
{
	Q_OBJECT

public:
	ControlerApplication(int &argc, char **argv);
	~ControlerApplication();

	bool initApp();
	ConfigWnd* mainWnd() const;

	static ControlerApplication *app();

private slots:
	void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void onExitApp();
	//void onClearCache();
	void onShowConfigWindow();
	void onConfigWndHided();

private:
	bool setSigleInstance();
	void createTrayIcon();
	void onClickTrayIcon();

private:
	TrayIcon* m_trayIcon;
	ConfigWnd* m_configWnd;
	ControlerServer m_server;
	QSharedMemory m_shareMem;
	
};