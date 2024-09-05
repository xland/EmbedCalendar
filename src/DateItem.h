#pragma once
#include <string>

class DateItem
{
public:
	DateItem() = default;
	~DateItem() = default;
public:
	std::string date;
	std::string lunar;
	std::string badge;
	bool hasSchdule;
	bool isActive;
	bool isToday;
	bool isCurrt;
	float dateX, dateY;
	float lunarX, lunarY;
	float badgeX, badgeY;
	float cX, cY;
	bool isHover;
private:

};