#pragma once
#include <QFont>
#include <QFontDatabase>

class Util
{
public:
	static QFont* getIconFont(const int& fontSize);
	static QFont* getTextFont(const int& fontSize);
};

