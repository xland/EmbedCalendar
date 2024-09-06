#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>
#include <vector>

#include "../ListItem.h"
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
	void SetText(std::vector<ListItem>&& param);
private:
	void measureEmpty();
	void measureList();
	void paintEmpty(SkCanvas* canvas);
	void paintList(SkCanvas* canvas);
	void paintItem(SkCanvas* canvas,const size_t& index);
	void paintItemBg(SkCanvas* canvas, const size_t& index);
	void paintScroller(SkCanvas* canvas);
private:
	std::vector<ListItem> items;
	float emptyFontSize;
	std::string emptyText;
	float emptyX, emptyY;
	SkRect listRect;
	float itemHeight;
	float scrollTop{ 0 };
	int hoverIndex{ -1 };
	float itemFontSize1,itemFontSize2;
};

