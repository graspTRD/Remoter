#include "stdafx.h"
#include "keyboardaction.h"

KeyboardAction::KeyboardAction( int unicode )
	:m_unicode(unicode)
{
	m_type = KEYBOARD;
}


KeyboardAction::~KeyboardAction()
{

}


KeyboardAction* KeyboardAction::parse( const QByteArray& data )
{
	KeyboardAction* keyAction = new KeyboardAction(0);
	QByteArray iData = data.right(data.size() - 1);
	keyAction->m_unicode = GetDataFromStream<int>(&iData);
	return keyAction;
}
