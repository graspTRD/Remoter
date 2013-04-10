#pragma once
#include "controleraction.h"

class ShortcutGroupAction : public ControlerAction
{
	Q_OBJECT
public:

	ShortcutGroupAction(int mode, int key1, int key2);
	virtual ~ShortcutGroupAction(void);
	void setMode(int mode);
	virtual QByteArray toByteArray();

	static const int SHORTCUT_START = 0;
	static const int SHORTCUT_END = 1;
	static ShortcutGroupAction* parse(const QByteArray& data);

public: 
	int m_mode;
	int m_firstKey;
	int m_lastKey;
};
