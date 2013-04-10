#ifndef TRAYICON_H
#define TRAYICON_H
#include <QSystemTrayIcon>
#include <QMenu>

class TrayIcon: public QSystemTrayIcon
{
	Q_OBJECT

public:
	TrayIcon();
	virtual ~TrayIcon();
	void configWndHided();

signals:
	
	void showConfigWnd();
	//void clearCache();
	void exitApp();

private slots:
	void onTriggerConfigWnd();

private:
	void initTrayMenu();
	QMenu* m_trayMenu;
	QAction* m_pConfigWndAction;
	QAction* m_p;
	QAction* m_pExitAppAction;
};

#endif //TRAYICON_H