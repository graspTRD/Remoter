#include "stdafx.h"
#include "keycodeconverter.h"
#include "Actions/keyboardaction.h"

QHash<int, int> KeyCodeConverter::m_codeMap;

KeyCodeConverter::KeyCodeConverter(void)
{
}


KeyCodeConverter::~KeyCodeConverter(void)
{
}

int KeyCodeConverter::convert( int unicode )
{
	initMap();
	return m_codeMap.value(unicode, NO_SWING_KEYCODE);
}

void KeyCodeConverter::initMap()
{
	if(!m_codeMap.isEmpty()) return;
	initIntervals();
	initOthers();
}

void KeyCodeConverter::initIntervals()
{
	initInterval(97, 122, VK_A); //65 == VK_A
	initInterval(65, 90, VK_A);
	//initInterval(48, 57, VK_NUMPAD0); //VK_0 == 48
	initInterval(48, 57, VK_0); //VK_0 == 48
}

void KeyCodeConverter::initInterval( int unicodeStart, int unicodeStop, int localKeyCodeStart )
{
	int shift = localKeyCodeStart - unicodeStart;

	for (int i = unicodeStart; i <= unicodeStop; i++)
	{
		m_codeMap.insert(i, i + shift);
	}
}

void KeyCodeConverter::initOthers()
{
	m_codeMap.insert(10, VK_RETURN); //»Ø³µ
	m_codeMap.insert(KeyboardAction::UNICODE_BACKSPACE, VK_BACK); //É¾³ý
	
	m_codeMap.insert(513, VK_LEFT);
	m_codeMap.insert(514, VK_UP);
	m_codeMap.insert(515, VK_RIGHT);
	m_codeMap.insert(516, VK_DOWN);
	m_codeMap.insert(517, VK_ESCAPE); //esc 
	m_codeMap.insert(518, VK_LWIN);
	m_codeMap.insert(519, VK_TAB);
	m_codeMap.insert(520, VK_MENU); //alt
	m_codeMap.insert(521, VK_OEM_PLUS);
	m_codeMap.insert(522, VK_OEM_MINUS);
	m_codeMap.insert(523, VK_CLOSE);
	m_codeMap.insert(524, VK_SPACE);
}

bool KeyCodeConverter::useShift( int unicode )
{
	return (unicode >= VK_A && unicode <= VK_Z) ||
		(unicode >= VK_0 && unicode <= VK_9);
}
