﻿#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>
#include <vector>
#include <rapidjson/document.h>
#include "../ListItem.h"
class ListBody
{
public:
	ListBody() = default;
	~ListBody() = default;
	static void Init();
	static ListBody* Get();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnLeftBtnUp(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
	void OnMouseWheel(const int& span);
	void SetData(rapidjson::Value& data);
private:
	void measureList();
	void paintEmpty(SkCanvas* canvas);
	void paintList(SkCanvas* canvas);
	void paintItem(SkCanvas* canvas,const size_t& index);
	void paintItemBg(SkCanvas* canvas, const size_t& index);
	void paintItemBtn(SkCanvas* canvas, const size_t& index);
	void paintScroller(SkCanvas* canvas);
	void caculateTop();
	void clipText(ListItem& item);
private:
	std::string tip;
	SkRect tipRect;
	float tipX, tipY;

	std::vector<ListItem> items;
	float emptyFontSize;
	float itemBtnLeft;
	std::string emptyText;
	float emptyX, emptyY;
	SkRect listRect;
	float itemHeight;
	float scrollTop{ 0 }, thumbTop{0};
	int hoverIndex{ -1 };
	float itemFontSize1,itemFontSize2;
	float listHeight;
	float thumbHeight, thumbWidth, thumbDragStartY{-1.0};
	bool mouseInListRect{ false };
	bool mouseInThumbRect{ false };
	bool mouseInBtnRect{ false };
};

