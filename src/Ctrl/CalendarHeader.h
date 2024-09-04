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
	void SetText(const std::string& text);
public:
private:
	void measure();
	bool isInCircle(float px, float py, const SkPoint& center);
private:
	std::string yearMonthStr;
	SkPoint c1Center, c2Center;
	SkPoint textPos;
	SkPoint icon1Pos, icon2Pos;
	const char* leftIcon{ (const char*)u8"\ue709" };
	const char* rightIcon{ (const char*)u8"\ue70e" };
	float textSize{ 28 };
	float iconSize{ 14 };
	float r;
	bool mouseInLeft{ false }, mouseInRight{ false };
};