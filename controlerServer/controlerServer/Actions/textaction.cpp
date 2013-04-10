
#include "stdafx.h"
#include "textaction.h"


TextAction::TextAction(const QString& passWord)
	:m_text(passWord)
{
	m_type = TEXT;
}


TextAction::~TextAction()
{
}

TextAction* TextAction::parse( const QByteArray& data )
{
	QString text = readStringFromData(&data.mid(1));
	return new TextAction(text);
}
