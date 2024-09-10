#pragma once

#include <string>

class ListItem
{
public:
	ListItem() = default;
	~ListItem() = default;
public:
	std::string title;
	std::string desc;
	std::string calendarNo;
	std::string scheduleNo;
	SkColor color;
	bool isAllowEdit;
	float y,w1,w2;
private:

};