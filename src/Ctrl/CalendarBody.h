#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>
#include <vector>

#include "../DateItem.h"
class CalendarBody
{
public:
	CalendarBody();
	~CalendarBody();

	static void Init();
	static CalendarBody* Get();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
	void SetText(std::vector<DateItem>&& param);
public:
	float l, t, b, r;
private:
	std::vector<DateItem> items;
	float rr,span;
	float dateSize, lunarSize, badgeSize;
	int hoverIndex{ -1 };
};