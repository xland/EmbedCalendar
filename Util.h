#pragma once
#include <QFont>
#include <QFontDatabase>

class Util
{
public:
	static QFont* getIconFont();
	static QFont* getTextFont(const int& fontSize);
};

