#pragma once

#include <QHash>

class KeyCodeConverter
{
public:
	KeyCodeConverter();
	~KeyCodeConverter();
	static int convert(int unicode);
	static bool useShift(int unicode);

private:
	static void initMap(); 
	static void initIntervals();
	static void initOthers();

	static void initInterval(int unicodeStart, int unicodeStop, int localKeyCodeStart);

public:
	static const int NO_SWING_KEYCODE = -2;

private:
	static QHash<int, int> m_codeMap;
};
