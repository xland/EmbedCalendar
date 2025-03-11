#pragma once
#include <Windows.h>
#include <QFont>
#include <QFontDatabase>

class Util
{
public:
	static void initDebuger();
	static QFont* getIconFont(const int& fontSize);
	static QFont* getTextFont(const int& fontSize);
	static HWND getWorkerW();
};

