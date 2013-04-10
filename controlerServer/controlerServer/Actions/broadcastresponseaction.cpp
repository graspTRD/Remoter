#include "stdafx.h"
#include "broadcastresponseaction.h"

BroadcastResponseAction::BroadcastResponseAction( QString& hostName )
	:m_hostName(hostName)
{

}

BroadcastResponseAction::~BroadcastResponseAction()
{

}

QByteArray BroadcastResponseAction::toByteArray()
{
	return m_hostName.toLocal8Bit();
}

BroadcastResponseAction* BroadcastResponseAction::parse( const QByteArray& data )
{
	QByteArray iData = data;
	QString hostName = readStringFromData(&iData);
	return new BroadcastResponseAction(hostName);
}

