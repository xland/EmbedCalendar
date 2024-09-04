#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>

class TitleBar
{
public:
	TitleBar();
	~TitleBar();
	static void Init();
	static TitleBar* Get();
	bool IsInCaption(const int& x, const int& y);
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
private:
	bool mouseInSettingBtn{false};
	bool mouseInPinBtn{ false };
	float fontSize{ 14 };
	SkRect settingRect;
	SkRect pinRect;
	SkRect dragRect;
};