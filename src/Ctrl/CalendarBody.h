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

private:
	std::vector<DateItem> items;
	float r,x,y,span;
	float numX1, numX2, numY;
	float textX1, textX2, textY;
	float badgeX, badgeY;
	float numSize, textSize1, textSize2;
};