#include "stdafx.h"
#include "authentificationresponseaction.h"

AuthentificationResponseAction::AuthentificationResponseAction( bool authentificated )
	:m_bAuthentificated(authentificated)
{
	m_type = AUTHENTIFICATION_RESPONSE;
}

AuthentificationResponseAction::~AuthentificationResponseAction()
{

}

QByteArray AuthentificationResponseAction::toByteArray()
{
	QByteArray outData;
	QDataStream os(&outData, QIODevice::ReadWrite);
	os<<m_type<<m_bAuthentificated;
	return outData;
}

