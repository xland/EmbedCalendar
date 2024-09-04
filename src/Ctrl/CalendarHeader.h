#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>

class CalendarHeader
{
public:
	CalendarHeader();
	~CalendarHeader();
	void Init();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
public:
	std::string yearMonthStr;
private:

};