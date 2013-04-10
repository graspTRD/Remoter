#include "stdafx.h"
#include "trayicon.h"

TrayIcon::TrayIcon()
	:m_trayMenu(NULL), m_pConfigWndAction(NULL), m_p(NULL), m_pExitAppAction(NULL)
{
	initTrayMenu();
}

TrayIcon::~TrayIcon()
{
	if(m_trayMenu)
	{
		delete m_trayMenu;
		m_trayMenu = NULL;
	}
}

void TrayIcon::initTrayMenu()
{
	if(m_trayMenu) return;

	m_trayMenu = new QMenu();
	m_pConfigWndAction = m_trayMenu->addAction("显示本机ip");
	//m_pConfigWndAction->setCheckable(true);
	//m_p = m_trayMenu->addAction("无功能");
	m_trayMenu->addSeparator();
	m_pExitAppAction = m_trayMenu->addAction("退出");

	connect(m_pConfigWndAction, SIGNAL(triggered()), this, SLOT(onTriggerConfigWnd()));
	//connect(m_p, SIGNAL(triggered()), this, SIGNAL(clearCache()));
	connect(m_pExitAppAction, SIGNAL(triggered()), this, SIGNAL(exitApp()));

	setContextMenu(m_trayMenu);
}

void TrayIcon::onTriggerConfigWnd()
{
	emit showConfigWnd();
}

void TrayIcon::configWndHided()
{
	//m_pConfigWndAction->setChecked(false);
}
