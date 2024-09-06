#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>

class ListHeader
{
public:
	ListHeader();
	~ListHeader();
	static void Init();
	static ListHeader* Get();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
public:
	std::string text;
	float btnX, btnY, btnSize;
private:
	float fontSize;
	float textX, textY, iconX;
	bool isMouseIn{ false };
};

