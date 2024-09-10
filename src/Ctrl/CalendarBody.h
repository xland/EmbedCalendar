#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>
#include <vector>
#include <rapidjson/document.h>

#include "../DateItem.h"
class CalendarBody
{
public:
	CalendarBody() = default;
	~CalendarBody() = default;

	static void Init();
	static CalendarBody* Get();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
	void SetData(rapidjson::Value& data);
public:
	float l, t, b, r;
private:
	void setItemsPos();
private:
	std::vector<DateItem> items;
	float rr,span;
	float dateSize, lunarSize, badgeSize;
	int hoverIndex{ -1 };
};