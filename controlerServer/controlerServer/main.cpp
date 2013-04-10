#include "stdafx.h"
#include "controlerapplication.h"
#include <QTextCodec>
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	ControlerApplication a(argc, argv);
	if(!a.initApp())
		exit(-1);
	return a.exec();
}
