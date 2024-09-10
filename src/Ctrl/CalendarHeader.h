#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>
#include <rapidjson/document.h>

class CalendarHeader
{
public:
	CalendarHeader() = default;
	~CalendarHeader() = default;
	static void Init();
	static CalendarHeader* Get();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
	void SetData(rapidjson::Value& data);
public:
	SkPoint c1Center, c2Center;
	float r;
private:
	void measure();
private:
	std::string toolTipLeft, toolTipRight;
	std::string yearMonthStr;
	SkPoint textPos;
	SkPoint icon1Pos, icon2Pos;
	const char* leftIcon{ (const char*)u8"\ue709" };
	const char* rightIcon{ (const char*)u8"\ue70e" };
	float textSize;
	float iconSize;
	bool mouseInLeft{ false }, mouseInRight{ false };
};