#include "stdafx.h"
#include "uiutils.h"


Uiutils::Uiutils(void)
{
}


Uiutils::~Uiutils(void)
{
}

BOOL CALLBACK hideZoomProc(HWND hwnd, LPARAM lParam)
{
	TCHAR title[1024];
	GetWindowText(hwnd, title, 1024);
	if(QString::fromWCharArray(title) == "放大镜")
	{
		//::SendMessage(hwnd, WM_CLOSE, 0, 0);
		::ShowWindow(hwnd, SW_HIDE);
	}
	return true;
}

BOOL CALLBACK killZoomProc(HWND hwnd, LPARAM lParam)
{
	TCHAR title[1024];
	GetWindowText(hwnd, title, 1024);
	if(QString::fromWCharArray(title) == "放大镜")
	{
		::SendMessage(hwnd, WM_CLOSE, 0, 0);
	}
	return true;
}

void Uiutils::killZoom()
{
	::EnumWindows(killZoomProc, 0);
}

void Uiutils::hideZoom()
{
	::EnumWindows(hideZoomProc, 0);
}
