#include "stdafx.h"
#include "controlerapplication.h"
#include "host/host.h"
#include <QDir>
#include <QPluginLoader>
#include <QAbstractNetworkCache>


ControlerApplication::ControlerApplication(int &argc, char **argv)
	: QApplication(argc,argv),
	m_trayIcon(NULL), m_configWnd(NULL)
{
	Host::getInstance();
	m_configWnd = new ConfigWnd();
	connect(m_configWnd, SIGNAL(windowHided()), this, SLOT(onConfigWndHided()));
}

ControlerApplication::~ControlerApplication()
{
	Free_Ptr(m_trayIcon);
	Free_Ptr(m_configWnd);
}

ControlerApplication *ControlerApplication::app()
{
	return (static_cast<ControlerApplication *>(QCoreApplication::instance()));
}

bool ControlerApplication::setSigleInstance()
{
	m_shareMem.setKey(QString("PCControler"));
	if(m_shareMem.attach())
	{
		Error::showError("手机遥控 已经在运行", "系统提示"); //;
		return false;
	}
	if (!m_shareMem.create(1))
	{
		Error::showError("无法创建共享内存", "系统提示"); //;
		return false;
	}

	return true;
}

bool ControlerApplication::initApp()
{
	Host *host = &Host::getInstance();
	if(!host->init()) 
	{
		Error::showError("Host初始化失败");
		return false;
	}

	// 	bool ret = Host::setAppStyle(":/images/defaultStyle");
	// 	if(!ret) return false;

	//start Server
	if(!m_server.listen(53399))
	{
		qDebug()<<"启动服务器失败！";
		return false;
	}

	createTrayIcon();
	return true;
}

void ControlerApplication::createTrayIcon()
{
	if(m_trayIcon) return;
	m_trayIcon = new TrayIcon();
	m_trayIcon->setToolTip("手机遥控");
	m_trayIcon->setIcon(QIcon(":/Controler/Resources/pccontroler.png"));

	connect(m_trayIcon, SIGNAL(showConfigWnd()), this, SLOT(onShowConfigWindow()));
	//connect(m_trayIcon, SIGNAL(clearCache()), this, SLOT(onClearCache()));
	connect(m_trayIcon, SIGNAL(exitApp()), this, SLOT(onExitApp()));
	connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(onTrayIconActivated(QSystemTrayIcon::ActivationReason)));

	m_trayIcon->show();
}

void ControlerApplication::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:
		break;
	case QSystemTrayIcon::DoubleClick:
		onClickTrayIcon();
		break;
	case QSystemTrayIcon::MiddleClick:
		break;
	case QSystemTrayIcon::Context:
		break;
	default:;
	}
}

void  ControlerApplication::onClickTrayIcon()
{
	if(!m_configWnd) return;

	if(!m_configWnd->isVisible())
	{
		m_configWnd->showNormal();
		m_configWnd->activateWindow();
	}
	else
	{
		m_configWnd->hide();
		//onConfigWndHided();
	}
}

void ControlerApplication::onShowConfigWindow()
{
	if(!m_configWnd) return;

	if(!m_configWnd->isVisible())
	{
		m_configWnd->showNormal();
		m_configWnd->activateWindow();
	}
	else
	{
		m_configWnd->hide();
		//onConfigWndHided();
	}
}

/*
void ControlerApplication::onClearCache()
{

}*/

void ControlerApplication::onExitApp()
{
	QCoreApplication::quit();
}

ConfigWnd* ControlerApplication::mainWnd() const
{
	return m_configWnd;
}

void ControlerApplication::onConfigWndHided()
{
	if(!m_configWnd) return;
	//m_trayIcon->configWndHided();
}
