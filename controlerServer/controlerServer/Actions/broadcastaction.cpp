#include "stdafx.h"
#include "broadcastaction.h"

BroadcastAction::BroadcastAction( QString& content )
	:m_content(content)
{

}

BroadcastAction::~BroadcastAction()
{

}

QByteArray BroadcastAction::toByteArray()
{
	return m_content.toLocal8Bit();
}

BroadcastAction* BroadcastAction::parse( const QByteArray& data )
{
	QByteArray iData = data;
	QString content = readStringFromData(&iData);
	return new BroadcastAction(content);
}
