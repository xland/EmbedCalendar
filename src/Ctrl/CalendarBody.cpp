#include <format>
#include "CalendarBody.h"
#include "WeekHeader.h"
#include "SettingMenu.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../TypeDefine.h"
#include "../Skin.h"
#include "../Util.h"
#include "../WsConn.h"

namespace {
	std::unique_ptr<CalendarBody> calendarBody;
}

void CalendarBody::Init()
{
	calendarBody = std::make_unique<CalendarBody>();
	auto win = MainWin::Get();
	win->paintHandlers.push_back(std::bind(&CalendarBody::OnPaint, calendarBody.get(), std::placeholders::_1));
	win->dpiHandlers.push_back(std::bind(&CalendarBody::OnDpi, calendarBody.get()));
	win->mouseMoveHandlers.push_back(std::bind(&CalendarBody::OnMouseMove, calendarBody.get(), std::placeholders::_1, std::placeholders::_2));
	win->leftBtnDownHandlers.push_back(std::bind(&CalendarBody::OnLeftBtnDown, calendarBody.get(), std::placeholders::_1, std::placeholders::_2));
}

CalendarBody* CalendarBody::Get()
{
	return calendarBody.get();
}

void CalendarBody::OnPaint(SkCanvas* canvas)
{
	auto win = MainWin::Get();
	auto skin = Skin::Get();
	auto font = Font::GetText();
	SkPaint paint;
	paint.setAntiAlias(true);
	for (size_t i = 0; i < items.size(); i++)
	{
		auto& item = items[i];
		if (item.isActive) {
			paint.setColor(0xFFF02C38);
			canvas->drawCircle(item.cX, item.cY, rr, paint);
		}
		else if(i == hoverIndex)
		{
			paint.setColor(skin->hoverBg);
			canvas->drawCircle(item.cX, item.cY, rr, paint);
		}
		if (item.isToday) {
			paint.setColor(0xFFF02C38);
			paint.setStroke(true);
			paint.setStrokeWidth(1.6);
			canvas->drawCircle(item.cX, item.cY, rr, paint);
			paint.setStroke(false);
		}
		SkColor tColor{ skin->text2 };
		if (item.isActive) {
			tColor = 0xFFFFFFFF;
		}
		else if (item.isCurrt) {
			tColor = skin->text0;
		}
		font->setSize(dateSize);
		paint.setColor(tColor);
		canvas->drawString(item.date.data(),item.dateX, item.dateY, *font, paint);

		paint.setColor(item.isActive ? 0xFFFFFFFF : skin->text1);
		font->setSize(lunarSize);

		auto lunarStr = Util::ToWStr(item.lunar.data());

		canvas->drawString(item.lunar.data(),item.lunarX,item.lunarY, *font, paint);

		paint.setColor(item.isActive ? 0xFFFFFFFF : skin->text1);
		font->setSize(badgeSize);
		canvas->drawString(item.badge.data(),item.badgeX,item.badgeY, *font, paint);

		if (item.hasSchdule) {
			paint.setColor(tColor);
			canvas->drawCircle(item.cX, item.cY + 23 * win->dpi, 3 * win->dpi, paint);
		}
	}
}

void CalendarBody::OnDpi()
{
	auto win = MainWin::Get();
	rr = 28 * win->dpi;
	l = 30 * win->dpi;
	t = WeekHeader::Get()->bottom + 6 * win->dpi;
	b = t + 6 * 2 * win->dpi + 6 * (2 * rr);
	r = win->w - l;
	span = (win->w - 20 * win->dpi * 2) / 7;

	dateSize = 20 * win->dpi;
	lunarSize = 11 * win->dpi;
	badgeSize = 10 * win->dpi;

	setItemsPos();
}

void CalendarBody::setItemsPos()
{
	auto win = MainWin::Get();
	auto font = Font::GetText();
	SkRect measureRect;
	SkPoint pos{ SkPoint::Make(l,t) };
	for (size_t yIndex = 0; yIndex < 6; yIndex++)
	{
		pos.fY += 2 * win->dpi;
		for (size_t xIndex = 0; xIndex < 7; xIndex++)
		{
			auto& item = items[yIndex * 7 + xIndex];
			item.cX = pos.fX + rr;
			item.cY = pos.fY + rr;
			font->setSize(dateSize);
			font->measureText(item.date.data(), item.date.size(), SkTextEncoding::kUTF8, &measureRect);
			item.dateX = pos.fX + rr - measureRect.width() / 2 - measureRect.fLeft;
			item.dateY = pos.fY + 0 - measureRect.fTop + 12 * win->dpi;


			font->setSize(lunarSize);
			font->measureText(item.lunar.data(), item.lunar.size(), SkTextEncoding::kUTF8, &measureRect);
			item.lunarX = pos.fX + rr - measureRect.width() / 2 - measureRect.fLeft;
			item.lunarY = pos.fY + 0 - measureRect.fTop + 32 * win->dpi;


			font->setSize(badgeSize);
			font->measureText(item.badge.data(), item.badge.size(), SkTextEncoding::kUTF8, &measureRect);
			item.badgeX = pos.fX + 0 - measureRect.width() / 2 - measureRect.fLeft + 46 * win->dpi - 2;
			item.badgeY = pos.fY + (0 - measureRect.fTop + 8 * win->dpi);

			pos.fX += span;
		}
		pos.fY += 2 * rr;
		pos.fX = l;
	}
}

void CalendarBody::OnLeftBtnDown(const int& x, const int& y)
{
	if (hoverIndex < 0)return;
	auto str = std::format(R"({{"msgName":"changeDate","data": {{"year":{},"month":{},"date":{}}}}})",
		items[hoverIndex].year, items[hoverIndex].month, items[hoverIndex].date);
	WsConn::Get()->PostMsg(std::move(str));
}

void CalendarBody::OnMouseMove(const int& x, const int& y)
{
	if (x<l || x>r || y<t || y>b) {
		if (hoverIndex>=0) {
			hoverIndex = -1;
			MainWin::Cursor(IDC_ARROW);
			auto win = MainWin::Get();
			win->Refresh();
		}
		return;
	}
	auto settingMenu = SettingMenu::Get();
	if (settingMenu->visible && settingMenu->bg.contains(x, y)) {
		return;
	}
	int index{-1};
	SkPoint mousePos{ SkPoint::Make(x,y) };
	for (size_t i = 0; i < items.size(); i++)
	{
		SkPoint center{ SkPoint::Make(items[i].cX,items[i].cY) };
		bool flag = Util::IsInCircle(center, rr, mousePos);
		if (flag) {
			index = i;
			break;
		}
	}
	if (index != hoverIndex) {
		hoverIndex = index;
		auto win = MainWin::Get();
		win->Refresh();
	}
	if (hoverIndex>=0) {
		MainWin::Cursor(IDC_HAND);
	}
	else {
		MainWin::Cursor(IDC_ARROW);
	}
}

void CalendarBody::SetData(rapidjson::Value& data)
{
	auto arr = data["viewData"].GetArray();
	std::string currtStr = Util::ToStr(L"currt");
	for (auto& data : arr)
	{
		DateItem item;
		item.date = std::to_string(data["date"].GetUint());
		item.year = std::to_string(data["year"].GetUint());
		item.month = std::to_string(data["month"].GetUint());
		//auto str = Util::ToWStr(jsonStr.data());
		item.hasSchdule = data.HasMember("hasSchdule") ? data["hasSchdule"].GetBool() : false;
		item.isActive = data["isActive"].GetBool();
		item.isToday = data["isToday"].GetBool();
		item.lunar = data["lunarInfo"].GetString();
		item.isCurrt = data["type"].GetString() == currtStr;
		item.badge = data["docStatus"].GetString();
		items.push_back(std::move(item));
	}
	setItemsPos();
}
