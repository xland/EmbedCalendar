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
private:

};