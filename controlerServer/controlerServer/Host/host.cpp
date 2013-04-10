#include "stdafx.h"
#include "host.h"
#include "controls/servermainwnd.h"
#include <QTextCodec>

QHash<QString, Host::Creator> Host::objectTable;

Host::Host(void)
{
	initCodec();
}

Host::~Host(void)
{
}

void Host::initCore()
{
// 	//注册各个自定义类
// 	R_REGISTERCLASS_NAME(Window,ServerMainWnd);
}

void Host::initCodec()
{
	QTextCodec *codec = QTextCodec::codecForName("System");    
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForTr(codec);
}

bool Host::init()
{
//	initCore();
	return true;
}

bool Host::registerClass(const QString& className, Creator objCreator)
{
	if(objectTable.contains(className)) return false;
	QHash<QString, Creator>::iterator it = objectTable.insert(className, objCreator);
	return it == objectTable.end()? false : true;
}

Host::Creator Host::getCreator(const QString& key) 
{
	if(!objectTable.contains(key)) return NULL;
	return objectTable.value(key);
}

Host& Host::getInstance() 
{
	static Host _h;

	return _h;
}

bool Host::setAppStyle(const QString& styleName)
{
	QFile file(styleName);
	if(!file.open(QIODevice::ReadOnly)) return false;
	QString str = file.readAll();
	DebugError(str);
	qApp->setStyleSheet(str);
	return true;
}