#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>

class CalendarHeader
{
public:
	CalendarHeader();
	~CalendarHeader();
	static void Init();
	static CalendarHeader* Get();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
public:
	std::string yearMonthStr;
private:

};