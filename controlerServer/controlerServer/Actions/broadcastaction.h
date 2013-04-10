#ifndef BROADCASTACTION_H
#define BROADCASTACTION_H

#include "controleraction.h"
class BroadcastAction : public ControlerAction
{
	Q_OBJECT
public:
	BroadcastAction(QString& content);
	virtual ~BroadcastAction();
	virtual QByteArray toByteArray();
	static BroadcastAction* parse(const QByteArray& data);

public:
	QString m_content;
};

#endif //BROADCASTACTION_H