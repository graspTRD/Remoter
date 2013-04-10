
#ifndef ROKHCORE_H
#define ROKHCORE_H

#include <QString>
#include <QByteArray>
#include <QDataStream>

#define VK_A 65
#define VK_Z 90
#define VK_0 48
#define VK_9 57
#define VK_CLOSE 523

#define R_DISABLE_COPY(classname) \
	private: \
	classname(const classname &);\
	classname & operator = (const classname &);

#define R_DYNAMIC_CREATE(classname) \
	public: \
	static QObject* classname##Creator() \
	{ return new classname(); }


#define Free_Ptr(ptr) if(ptr){ delete ptr; ptr = NULL;}
#define Free_Ptr_Later(ptr) if(ptr){ ptr->deleteLater(); ptr = NULL;}
#define Create_Obj(classNameAndArguments) new classNameAndArguments

#ifdef _DEBUG 
	#define DebugError(msg) Error::debugError(msg);
#else
	#define DebugError(msg)
#endif


template <typename T>
T GetDataFromStream(QByteArray* data)
{
	QDataStream stream(data, QIODevice::ReadOnly);
	T s;
	stream>>s;
	return s;
}

#endif //ROKHCORE_H