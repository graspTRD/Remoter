#ifndef MOUSECLICKACTION_H
#define MOUSECLICKACTION_H

#include "controleraction.h"

class MouseClickAction : public ControlerAction
{
	Q_OBJECT
	typedef unsigned char byte;
public:
	MouseClickAction(QObject* parent = 0);
	MouseClickAction(byte button, bool state);
	virtual ~MouseClickAction();
	static MouseClickAction* parse(const QByteArray& data);

public:
	 static const byte BUTTON_NONE = 0;
	 static const byte BUTTON_LEFT = 1;
	 static const byte BUTTON_RIGHT = 2;
	 static const byte BUTTON_MIDDLE = 3;
	 static const bool STATE_UP = false;
	 static const bool STATE_DOWN = true;

	 byte m_button;
	 bool m_state;
};

#endif //MOUSECLICKACTION_H


