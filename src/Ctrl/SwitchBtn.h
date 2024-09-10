#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>

class SwitchBtn
{
public:
	SwitchBtn() = default;
	~SwitchBtn() = default;
	static void Init();
	static SwitchBtn* Get();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
public:
	bool listVisible{ false };
	SkRect rect;
private:
	float fontSize;
	float textX, textY, iconX;
	bool isMouseIn{ false };
};

