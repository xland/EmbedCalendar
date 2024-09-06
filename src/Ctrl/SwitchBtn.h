#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>

class SwitchBtn
{
public:
	SwitchBtn();
	~SwitchBtn();
	static void Init();
	static SwitchBtn* Get();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
private:
	SkRect rect;
	float fontSize;
	float textX, textY, iconX;
	bool isMouseIn{ false };
	bool listVisible{ false };
};

