#include "stdafx.h"
#include "mousewheelaction.h"


// MouseWheelAction::MouseWheelAction(QObject* parent /*= 0*/)
// 	:ControlerAction(parent)
// {
// }

MouseWheelAction::MouseWheelAction( byte amount )
	:m_amount(amount)
{
	m_type = MOUSE_WHEEL;
}


MouseWheelAction::~MouseWheelAction()
{
}

MouseWheelAction* MouseWheelAction::parse( const QByteArray& data )
{
	MouseWheelAction* mwAction = new MouseWheelAction((byte)0);
	QByteArray iData = data.right(data.size() - 1);
	
	mwAction->m_amount = static_cast<byte>(GetDataFromStream<byte>(&iData));
	mwAction->m_bValid = true;

	return mwAction;
}
