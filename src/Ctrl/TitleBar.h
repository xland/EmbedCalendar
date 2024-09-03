#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>

class TitleBar
{
public:
	TitleBar();
	~TitleBar();
	void Init();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnLeftBtnUp(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
	void OnMouseDrag(const int& x, const int& y);
private:
	bool mouseInSettingBtn{false};
	bool mouseInPinBtn{ false };
	float fontSize{ 14 };
	SkRect settingRect;
	SkRect pinRect;
};