#ifndef MOUSEMOVEACTION_H
#define MOUSEMOVEACTION_H

#include "controleraction.h"

class MouseMoveAction : public ControlerAction
{
	Q_OBJECT;
public:
	MouseMoveAction(short moveX, short moveY);
	virtual ~MouseMoveAction();

	short getMoveX() const;
	short getMoveY() const;
	static MouseMoveAction* parse(const QByteArray& data);

public:
	short m_moveX;
	short m_moveY;
};

#endif //MOUSEMOVEACTION_H



