#include "stdafx.h"
#include "shortcutgroupaction.h"

ShortcutGroupAction::ShortcutGroupAction(int mode, int key1, int key2 )
	:m_firstKey(key1), m_lastKey(key2), m_mode(mode)
{
	m_type = SHORTCUTGROUP;
}


ShortcutGroupAction::~ShortcutGroupAction(void)
{

}

void ShortcutGroupAction::setMode( int mode )
{
	m_mode = mode;
}

QByteArray ShortcutGroupAction::toByteArray()
{
	QByteArray outData;
	QDataStream os(&outData, QIODevice::ReadWrite);
	os<<m_type<<m_mode<<m_firstKey<<m_lastKey;
	return outData;
}

ShortcutGroupAction* ShortcutGroupAction::parse( const QByteArray& data )
{
	QByteArray ida;
	int mode = GetDataFromStream<int>(&data.mid(1));
	int key1 = GetDataFromStream<int>(&data.mid(5));
	int key2 = GetDataFromStream<int>(&data.mid(9));

	return new ShortcutGroupAction(mode, key1, key2);
}
