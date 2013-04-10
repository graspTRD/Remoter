#include "stdafx.h"
#include "mouseclickaction.h"

// MouseClickAction::MouseClickAction(QObject* parent /*= 0*/)
// 	:ControlerAction(parent)
// {
// }

MouseClickAction::MouseClickAction( byte button, bool state )
	:m_button(button), m_state(state)
{
	m_type = MOUSE_CLICK;
}


MouseClickAction::~MouseClickAction()
{
}

MouseClickAction* MouseClickAction::parse( const QByteArray& data )
{
	MouseClickAction* mcAction = new MouseClickAction(0, false);
	QByteArray iData = data.right(data.size() - 1);

	mcAction->m_button = iData.at(0);
	mcAction->m_state = iData.at(1);
	mcAction->m_bValid = true;

	return mcAction;
}
