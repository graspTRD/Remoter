#ifndef AUTHENTIFICATIONACTION_H
#define AUTHENTIFICATIONACTION_H

#include "controleraction.h"

class AuthentificationAction : public ControlerAction
{
	Q_OBJECT
public:
	AuthentificationAction(const QString& passWord);
	virtual ~AuthentificationAction();
	static AuthentificationAction* parse(const QByteArray& data);

public:
	QString m_passWord;
};

#endif //AUTHENTIFICATIONACTION_H



