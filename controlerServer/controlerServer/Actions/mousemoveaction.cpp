#include "stdafx.h"
#include "mousemoveaction.h"

MouseMoveAction::MouseMoveAction( short moveX, short moveY )
	:m_moveX(moveX), m_moveY(moveY)
{
	m_type = MOUSE_MOVE;
}


MouseMoveAction::~MouseMoveAction()
{
}

short MouseMoveAction::getMoveX() const
{
	return m_moveX;
}

short MouseMoveAction::getMoveY() const
{
	return m_moveY;
}

MouseMoveAction* MouseMoveAction::parse( const QByteArray& data )
{
	MouseMoveAction* mAction = new MouseMoveAction(0, 0);
	QByteArray iData = data.right(data.size() - 1);
	mAction->m_moveX = GetDataFromStream<short>(&iData.left(2));
	mAction->m_moveY = GetDataFromStream<short>(&iData.right(2));
// 	qDebug()<<"Mouse Move:";
// 	qDebug()<<"X: "<<mAction->m_moveX<<"Y: "<<mAction->m_moveY;
	return mAction;
}
