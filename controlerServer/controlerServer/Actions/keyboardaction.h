#ifndef KEYBOARDACTION_H
#define KEYBOARDACTION_H

#include "controleraction.h"

class KeyboardAction : public ControlerAction
{
	Q_OBJECT
public:
	KeyboardAction(int unicode);
	virtual ~KeyboardAction();
	static KeyboardAction* parse(const QByteArray& data);

public:
	static const int UNICODE_BACKSPACE = -1;

public:
	int m_unicode;
};

#endif //KEYBOARDACTION_H


