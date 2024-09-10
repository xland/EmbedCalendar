#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>

class TitleBar
{
public:
	TitleBar() = default;
	~TitleBar() = default;
	static void Init();
	static TitleBar* Get();
	bool IsInCaption(const int& x, const int& y);
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
public:
	SkRect dragRect;
	SkRect settingRect;
	SkRect pinRect;
private:
	bool mouseInSettingBtn{false};
	bool mouseInPinBtn{ false };
	float fontSize;
	const char* settingIcon{ (const char*)u8"\ue6e8" };
	const char* pinIcon{ (const char*)u8"\ue70c" };
	SkPoint pinPos;
	SkPoint settingPos;
};