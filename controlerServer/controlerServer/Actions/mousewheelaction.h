#ifndef MOUSEWHEELACTION_H
#define MOUSEWHEELACTION_H

#include "controleraction.h"

class MouseWheelAction : public ControlerAction
{
	Q_OBJECT
public:
	MouseWheelAction(QObject* parent = 0);
	MouseWheelAction(byte amount);
	virtual ~MouseWheelAction();

	static MouseWheelAction* parse(const QByteArray& data);
	
public:
	byte m_amount;
};

#endif //MOUSEWHEELACTION_H


