#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>
class ListBody
{
public:
	ListBody();
	~ListBody();
	static void Init();
	static ListBody* Get();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
private:
	float emptyFontSize;
	std::string emptyText;
	float emptyX, emptyY;
};

