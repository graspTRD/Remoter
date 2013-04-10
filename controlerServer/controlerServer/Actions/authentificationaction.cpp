
#include "stdafx.h"
#include "authentificationaction.h"


AuthentificationAction::AuthentificationAction(const QString& passWord)
	:m_passWord(passWord)
{
	m_type = AUTHENTIFICATION;
}


AuthentificationAction::~AuthentificationAction()
{
}

AuthentificationAction* AuthentificationAction::parse( const QByteArray& data )
{
	QString passWord = readStringFromData(&data.mid(1));
	return new AuthentificationAction(passWord);
}
