#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRRect.h>
#include <include/core/SkRect.h>
#include <vector>
class SettingMenu
{
public:
	SettingMenu() = default;
	~SettingMenu() = default;
	static void Init();
	static SettingMenu* Get();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
	void Show();
public:
	std::vector<const char*> iconCode{
		(const char*)u8"\ue710", (const char*)u8"\ue70a", (const char*)u8"\ue711", (const char*)u8"\ue70d"
	};
	std::vector<std::string> menuText{"","","",""};
	bool visible{ false };
	SkRect bg;
private:
	float textLeftMargin, itemHeight,textStartY,margin;
	float fontSize;
	float radiu;
	int hoverIndex{ -1 };
};

