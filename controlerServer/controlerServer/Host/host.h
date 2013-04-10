#ifndef HOST_H
#define HOST_H

#include <QObject>
#include <QHash>
#include "rokhcore.h"

#define R_REGISTERCLASS(className) \
	registerClass(#className, className::className##Creator)

#define R_REGISTERCLASS_NAME(registerName,className) \
	registerClass(#registerName, className::className##Creator)

class Host
{
	R_DISABLE_COPY(Host)
	typedef QObject* (*Creator)();
public:
	virtual ~Host(void);
	static bool init();
	static Creator getCreator(const QString& key);
	static Host& getInstance();
	static bool setAppStyle(const QString& styleName);

private:
	Host(void);
	static void initCore();
	static void initCodec();
	static bool registerClass(const QString& className, Creator objCreator);
	static QHash<QString,Creator> objectTable;
};

//#include "host.cpp"

#endif

