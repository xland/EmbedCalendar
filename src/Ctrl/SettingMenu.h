#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>
class SettingMenu
{
public:
	SettingMenu();
	~SettingMenu();
	static void Init();
	static SettingMenu* Get();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
private:

};

