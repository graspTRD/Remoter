#ifndef BROADCASTRESPONSEACTION_H
#define BROADCASTRESPONSEACTION_H

#include "controleraction.h"
class BroadcastResponseAction : public ControlerAction
{
	Q_OBJECT
public:
	BroadcastResponseAction(QString& hostName);
	virtual ~BroadcastResponseAction();
	virtual QByteArray toByteArray();
	static BroadcastResponseAction* parse(const QByteArray& data);

public:
	QString m_hostName;
};

#endif //BROADCASTRESPONSEACTION_H