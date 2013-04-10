
#include"stdafx.h"
#include "errorutils.h"
#include <QMessageBox>

Error::Error(QString error = QString())
	:errorInfo(error)
{

}


Error::~Error()
{

}

void Error::throwError(const QString& error)
{
	showError(error);
	exit(1);
}

void Error::showError(const QString &error, const QString& title)
{
	QMessageBox::critical(0, title, error, QMessageBox::Ok, QMessageBox::Ok);
}

void Error::throwArgumentNull(const QString& paramName)
{
	if(paramName.isEmpty()) return;
	QString error = QString("参数%0不能为空").arg(paramName);
	showError(error);
}

void Error::checkAndThrowError(bool state, QString& error)
{
	if(state)  return;
	showError(error + "执行失败！");
}


#ifdef _DEBUG
	void Error::debugError(QString& error)
	{
		qDebug()<<error;
	}

	void Error::debugError(const char* error)
	{
		qDebug()<<QString("error");
	}
#endif