﻿#include <include/core/SkRRect.h>
#include <format>

#include "ListBody.h"
#include "ListHeader.h"
#include "SwitchBtn.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../TypeDefine.h"
#include "../Skin.h"
#include "../Util.h"
#include "../WsConn.h"

namespace {
	std::unique_ptr<ListBody> listBody;
}

void ListBody::Init()
{
	listBody = std::make_unique<ListBody>();
	auto win = MainWin::Get();
	win->paintHandlers.push_back(std::bind(&ListBody::OnPaint, listBody.get(), std::placeholders::_1));
	win->dpiHandlers.push_back(std::bind(&ListBody::OnDpi, listBody.get()));
	win->mouseMoveHandlers.push_back(std::bind(&ListBody::OnMouseMove, listBody.get(), std::placeholders::_1, std::placeholders::_2));
	win->mouseWheelHandlers.push_back(std::bind(&ListBody::OnMouseWheel, listBody.get(), std::placeholders::_1));
	win->leftBtnDownHandlers.push_back(std::bind(&ListBody::OnLeftBtnDown, listBody.get(), std::placeholders::_1, std::placeholders::_2));
	win->leftBtnUpHandlers.push_back(std::bind(&ListBody::OnLeftBtnUp, listBody.get(), std::placeholders::_1, std::placeholders::_2));
}

ListBody* ListBody::Get()
{
	return listBody.get();
}

void ListBody::OnPaint(SkCanvas* canvas)
{
	if (!SwitchBtn::Get()->listVisible) return;
	if (items.size()>0) {
		paintList(canvas);
	}
	else
	{
		paintEmpty(canvas);
	}
}

void ListBody::OnDpi()
{
	auto win = MainWin::Get();
	listRect.setLTRB(36 * win->dpi, 574 * win->dpi, win->w - 36 * win->dpi + thumbWidth, 790 * win->dpi);
	itemHeight = 48 * win->dpi;
	itemFontSize1 = 16 * win->dpi;
	itemFontSize2 = 14 * win->dpi;
	thumbWidth = 6 * win->dpi;
	itemBtnLeft = listRect.fRight - 48 * win->dpi;

	emptyFontSize = 18 * win->dpi;
	auto font = Font::GetText();
	SkRect measureRect;
	font->setSize(emptyFontSize);
	emptyText = Util::ToStr(L"暂无日程");
	font->measureText(emptyText.data(), emptyText.size(), SkTextEncoding::kUTF8, &measureRect);
	emptyX = win->w / 2 - measureRect.width() / 2 - measureRect.fLeft;
	emptyY = 680 * win->dpi;

	if (items.size() > 0) {
		measureList();
	}

	auto fontText = Font::GetText();
	fontText->setSize(itemFontSize2);
	fontText->measureText(tip.c_str(), tip.size(), SkTextEncoding::kUTF8, &measureRect);
	tipRect.setXYWH(listRect.fRight - 66*win->dpi,0,
		measureRect.width() + itemFontSize2, measureRect.height() + itemFontSize2);
	tipX = tipRect.fLeft + tipRect.width() / 2 - measureRect.width() / 2 - measureRect.fLeft;
	tipY = tipRect.fTop + tipRect.height() / 2 - measureRect.height() / 2 - measureRect.fTop;
}

void ListBody::clipText(ListItem& item)
{
	auto win = MainWin::Get();
	SkRect measureRect;
	auto font = Font::GetText();
	font->setSize(itemFontSize1);
	bool flag{ false };
	while (true)
	{
		font->measureText(item.title.data(), item.title.size(), SkTextEncoding::kUTF8, &measureRect);
		//auto s = Util::ToWStr(item.title.data());
		if (measureRect.width() < listRect.width() - thumbWidth - 46*win->dpi) {
			break;
		}
		Util::RemoveLastChar(item.title);
		if (item.title.empty()) {
			break;
		}
		flag = true;
	}
	if (flag) {
		item.title += "...";
	}
	flag = false;
	font->setSize(itemFontSize2);
	while (true)
	{
		font->measureText(item.desc.data(), item.desc.size(), SkTextEncoding::kUTF8, &measureRect);
		if (measureRect.width() < listRect.width() - thumbWidth - 12 * win->dpi) {
			break;
		}
		Util::RemoveLastChar(item.desc);
		if (item.desc.empty()) {
			break;
		}
		flag = true;
	}

	if (flag) {
		item.desc += "...";
	}
}

void ListBody::measureList()
{
	auto win = MainWin::Get();
	for (size_t i = 0; i < items.size(); i++)
	{
		items[i].y = listRect.fTop + i * 8 * win->dpi + i * itemHeight;
	}
	listHeight = items.back().y + itemHeight - listRect.fTop;
	if (listHeight <= listRect.height()) {
		return;
	}
	thumbHeight = listRect.height() / (listHeight / listRect.height());
	thumbHeight = std::max(thumbHeight, 60.f);
	for (size_t i = 0; i < items.size(); i++)
	{
		clipText(items[i]);
	}
}

void ListBody::paintEmpty(SkCanvas* canvas)
{
	auto fontText = Font::GetText();
	fontText->setSize(emptyFontSize);
	SkPaint paint;
	paint.setColor(Skin::Get()->text1);
	canvas->drawString(emptyText.data(), emptyX, emptyY, *fontText, paint);
}

void ListBody::paintList(SkCanvas* canvas)
{
	//SkPaint paint;
	//paint.setColor(Skin::Get()->text1);
	//canvas->drawRect(listRect, paint);
	canvas->save();
	canvas->clipRect(listRect);
	for (size_t i = 0; i < items.size(); i++)
	{
		paintItemBg(canvas, i);
		paintItem(canvas, i);
		paintItemBtn(canvas, i);
	}
	canvas->restore();


	auto win = MainWin::Get();
	auto skin = Skin::Get();
	if (mouseInBtnRect && hoverIndex >= 0 && items[hoverIndex].isAllowEdit) {
		SkPaint paint;
		paint.setAntiAlias(true);
		auto top{ items[hoverIndex].y - 20*win->dpi + scrollTop};
		paint.setColor(skin->toolTipBg);
		tipRect.offsetTo(tipRect.fLeft, top);
		auto rr = SkRRect::MakeRectXY(tipRect, 4 * win->dpi, 4 * win->dpi);
		canvas->drawRRect(rr, paint);
		auto fontText = Font::GetText();
		fontText->setSize(itemFontSize2);
		paint.setColor(skin->toolTipText);
		canvas->drawString(tip.c_str(), tipX, tipY + top, *fontText, paint);
	}

	paintScroller(canvas);	
}

void ListBody::paintItem(SkCanvas* canvas, const size_t& index)
{
	auto win = MainWin::Get();
	auto skin = Skin::Get();
	auto& item = items[index];
	SkPaint paint;
	auto fontText = Font::GetText();
	auto x = listRect.fLeft + 9 * win->dpi;
	fontText->setSize(itemFontSize1);
	paint.setColor(skin->text0);
	canvas->drawString(item.title.data(), x, item.y + scrollTop+ 20 * win->dpi, *fontText, paint);

	fontText->setSize(itemFontSize2);
	paint.setColor(skin->text2);
	canvas->drawString(item.desc.data(), x, item.y + scrollTop + 42 * win->dpi, *fontText, paint);
}

void ListBody::paintItemBg(SkCanvas* canvas, const size_t& index)
{
	SkPaint paint;
	paint.setAntiAlias(true);
	auto win = MainWin::Get();
	auto& item = items[index];
	if (index == hoverIndex) {
		SkRect rect;
		rect.setLTRB(listRect.fLeft, item.y + scrollTop, listRect.fRight-thumbWidth, item.y + scrollTop + itemHeight);
		auto rr = SkRRect::MakeRectXY(rect, 6 * win->dpi, 6 * win->dpi);
		paint.setColor(0x33FFFFFF);
		canvas->drawRRect(rr, paint);
	}
	paint.setColor(item.color);
	SkRect rect;
	rect.setXYWH(listRect.fLeft, item.y + scrollTop, 3 * win->dpi, itemHeight);
	SkVector radii[4]{ {6, 6}, //左上角
		{0, 0},  //右上角
		{0, 0},  //右下角
		{6, 6}  //左下角
	};
	SkRRect rr;
	rr.setRectRadii(rect, radii);
	canvas->drawRRect(rr, paint);
}

void ListBody::paintItemBtn(SkCanvas* canvas, const size_t& index)
{
	if (index != hoverIndex) return;
	auto& item = items[index];
	if (!item.isAllowEdit) return;
	auto win = MainWin::Get();
	auto skin = Skin::Get();
	SkPaint paint;
	paint.setColor(skin->text1);
	auto fontIcon = Font::GetIcon();
	fontIcon->setSize(itemFontSize2);
	const char* delCode{ (const char*)u8"\ue712" };
	auto top{ item.y + scrollTop + itemFontSize2*2 };
	canvas->drawString(delCode, listRect.fRight - 32 * win->dpi, top, *fontIcon, paint);
}

void ListBody::paintScroller(SkCanvas* canvas)
{
	if (listHeight <= listRect.height()) {
		return;
	}
	auto win = MainWin::Get();
	SkPaint paint;
	paint.setColor(0x08000000);
	auto rect = SkRect::MakeXYWH(listRect.fRight- thumbWidth, listRect.fTop, thumbWidth, listRect.height());
	canvas->drawRect(rect, paint);
	rect = SkRect::MakeXYWH(listRect.fRight- thumbWidth, listRect.fTop + thumbTop, thumbWidth,thumbHeight);
	paint.setColor(0x20000000);
	canvas->drawRect(rect, paint);
}

void ListBody::OnLeftBtnDown(const int& x, const int& y)
{
	if (!SwitchBtn::Get()->listVisible) return;
	if (mouseInThumbRect) {
		thumbDragStartY = y;
	}
	if (hoverIndex == -1) return;
	auto& item = items[hoverIndex];
	if (!item.isAllowEdit) return;
	auto win = MainWin::Get();
	if (x > itemBtnLeft) {
		auto msg = std::format(R"({{"msgName":"deleteSchedule","data":{{"scheduleNo":"{}","calendarNo":"{}"}}}})", item.scheduleNo, item.calendarNo);
		WsConn::Get()->PostMsg(std::move(msg));
	}
	else {
		auto msg = std::format(R"({{"msgName":"updateSchedule","data": {{"scheduleNo":"{}","calendarNo":"{}"}}}})", item.scheduleNo, item.calendarNo);
		WsConn::Get()->PostMsg(std::move(msg));
	}
}

void ListBody::OnLeftBtnUp(const int& x, const int& y)
{
	thumbDragStartY = -1.0;
}

void ListBody::OnMouseMove(const int& x, const int& y)
{
	if (!SwitchBtn::Get()->listVisible) return;
	if (items.size() == 0) return;
	auto win = MainWin::Get();
	if (thumbDragStartY >= 0) {
		auto distance = y - thumbDragStartY;
		thumbTop += distance;
		caculateTop();
		thumbDragStartY = y;
		win->Refresh();
	}
	mouseInListRect = listRect.contains(x, y);
	if (!mouseInListRect) {
		if (hoverIndex >= 0) {
			hoverIndex = -1;
			mouseInBtnRect = false;
			MainWin::Cursor(IDC_ARROW);
			win->Refresh();
		}
		return;
	}
	SkRect thumbRect = SkRect::MakeXYWH(listRect.fRight - thumbWidth, thumbTop + listRect.fTop, thumbWidth, thumbHeight);
	mouseInThumbRect = thumbRect.contains(x, y);	
	int index{ -1 };
	bool inBtn{ x > itemBtnLeft };
	for (size_t i = 0; i < items.size(); i++)
	{
		if (y>items[i].y+scrollTop && y<items[i].y+itemHeight+scrollTop && x < listRect.fRight - thumbWidth) {
			index = i;
			break;
		}
	}
	if (index >= 0 && inBtn != mouseInBtnRect) {
		mouseInBtnRect = inBtn;
		win->Refresh();
	}
	if (index != hoverIndex) {
		hoverIndex = index;
		win->Refresh();
	}
	if (hoverIndex >= 0) {
		MainWin::Cursor(IDC_HAND);
	}
	else {
		MainWin::Cursor(IDC_ARROW);
	}
}

void ListBody::caculateTop()
{
	if (thumbTop + listRect.fTop + thumbHeight > listRect.fBottom) {
		thumbTop = listRect.fBottom - thumbHeight - listRect.fTop;
	}
	if (thumbTop < 0) {
		thumbTop = 0;
	}
	scrollTop = 0 - (thumbTop / (listRect.height() - thumbHeight)) * (listHeight - listRect.height());
	
}

void ListBody::OnMouseWheel(const int& span)
{
	if (!SwitchBtn::Get()->listVisible) return;
	if (listHeight <= listRect.height()) {
		return;
	}
	if (!mouseInListRect) return;
	auto win = MainWin::Get();
	auto distance = 6 * win->dpi;
	if (span < 0) { //向下滚动
		thumbTop += distance;
	}
	else {
		thumbTop -= distance;
	}
	caculateTop();
	win->Refresh();
}

void ListBody::SetData(rapidjson::Value& data)
{
	auto arr = data["scheduleList"].GetArray();
	items.clear();
	for (auto& data : arr)
	{
		ListItem item;
		item.title = data["title"].GetString();
		item.desc = data["desc"].GetString();
		item.isAllowEdit = data["isAllowEdit"].GetBool();
		item.color = Util::ToColor(data["calendarColor"].GetString());
		item.calendarNo = data["calendarNo"].GetString();
		item.scheduleNo = data["scheduleNo"].GetString();
		items.push_back(std::move(item));
	}
	thumbTop = 0;
	scrollTop = 0;
	auto lang = data["lang"].GetObj();
	tip = lang["deleteSchedule"].GetString();
	if (listRect.isEmpty()) {
		OnDpi();
	}
	else {
		if (items.size() > 0) {
			measureList();
		}
	}
}
