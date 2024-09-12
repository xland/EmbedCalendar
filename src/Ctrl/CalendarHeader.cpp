

#include "CalendarHeader.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../WsConn.h"
#include "../TypeDefine.h"
#include "../Skin.h"
#include "TitleBar.h"
#include "../Util.h"

namespace {
	std::unique_ptr<CalendarHeader> calendarHeader;
}

void CalendarHeader::Init()
{
	calendarHeader = std::make_unique<CalendarHeader>();
	auto win = MainWin::Get();
	win->paintHandlers.push_back(std::bind(&CalendarHeader::OnPaint, calendarHeader.get(), std::placeholders::_1));
	win->dpiHandlers.push_back(std::bind(&CalendarHeader::OnDpi, calendarHeader.get()));
	win->mouseMoveHandlers.push_back(std::bind(&CalendarHeader::OnMouseMove, calendarHeader.get(), std::placeholders::_1, std::placeholders::_2));
	win->leftBtnDownHandlers.push_back(std::bind(&CalendarHeader::OnLeftBtnDown, calendarHeader.get(), std::placeholders::_1, std::placeholders::_2));
}

CalendarHeader* CalendarHeader::Get()
{
	return calendarHeader.get();
}

void CalendarHeader::OnPaint(SkCanvas* canvas)
{
	auto win = MainWin::Get();
	auto fontText = Font::GetText();
	auto fontIcon = Font::GetIcon();
	auto skin = Skin::Get();
	SkPaint paint;
	paint.setAntiAlias(true);
	fontText->setSize(textSize);
	paint.setColor(skin->text0);
	canvas->drawString(yearMonthStr.c_str(), textPos.fX, textPos.fY, *fontText, paint);
	if (mouseInLeft) {
		paint.setColor(skin->hoverBg);
		canvas->drawCircle(c1Center, r, paint);
	}
	if (mouseInRight) {
		paint.setColor(skin->hoverBg);
		canvas->drawCircle(c2Center, r, paint);
	}
	paint.setColor(skin->text1);
	paint.setStroke(true);
	paint.setStrokeWidth(1);
	canvas->drawCircle(c1Center, r, paint);
	canvas->drawCircle(c2Center, r, paint);	
	paint.setStroke(false);
	paint.setColor(skin->text0);
	fontIcon->setSize(iconSize);
	canvas->drawString(leftIcon, icon1Pos.fX, icon1Pos.fY, *fontIcon, paint);
	canvas->drawString(rightIcon, icon2Pos.fX, icon2Pos.fY, *fontIcon, paint);

	if(mouseInLeft){
		paint.setColor(skin->toolTipBg);
		auto rr = SkRRect::MakeRectXY(tipRectLeft, 4 * win->dpi, 4 * win->dpi);
		canvas->drawRRect(rr, paint);

		paint.setColor(skin->toolTipText);
		fontText->setSize(iconSize);
		canvas->drawString(tipLeft.c_str(), tipLeftX, tipLeftY, *fontText, paint);
	}
	if(mouseInRight){
		paint.setColor(skin->toolTipBg);
		auto rr = SkRRect::MakeRectXY(tipRectRight, 4 * win->dpi, 4 * win->dpi);
		canvas->drawRRect(rr, paint);

		paint.setColor(skin->toolTipText);
		fontText->setSize(iconSize);
		canvas->drawString(tipRight.c_str(), tipRightX, tipRightY, *fontText, paint);
	}
}

void CalendarHeader::OnDpi()
{
	measure();
	auto win = MainWin::Get();
	auto fontText = Font::GetText();
	auto titleBar = TitleBar::Get();





	SkRect measureRect;
	iconSize = 14 * win->dpi;
	r = 16 * win->dpi;
	c1Center.fY = titleBar->dragRect.fBottom + 6 * win->dpi + 32 * win->dpi / 2;
	c2Center.fY = c1Center.fY;

	c1Center.fX = win->w / 2 - 128 * win->dpi - r;
	c2Center.fX = win->w / 2 + 128 * win->dpi + r;

	auto fontIcon = Font::GetIcon();
	fontIcon->setSize(iconSize);
	int length = std::mbstowcs(nullptr, leftIcon, 0);
	fontIcon->measureText(leftIcon, length, SkTextEncoding::kUTF8, &measureRect);
	icon1Pos.fX = c1Center.fX - r + r - measureRect.width() / 2 - measureRect.fLeft;
	icon1Pos.fY = c2Center.fY - r + r - measureRect.height() / 2 - measureRect.fTop;

	icon2Pos.fY = icon1Pos.fY;
	icon2Pos.fX = c2Center.fX - (c1Center.fX - icon1Pos.fX);



	fontText->setSize(iconSize);
	fontText->measureText(tipLeft.c_str(), tipLeft.size(), SkTextEncoding::kUTF8, &measureRect);
	tipRectLeft.setXYWH(icon1Pos.fX - measureRect.width() / 2 - measureRect.fLeft,
		titleBar->dragRect.fBottom - measureRect.height() - iconSize, measureRect.width() + iconSize,
		measureRect.height() + iconSize);
	tipLeftX = tipRectLeft.fLeft + tipRectLeft.width() / 2 - measureRect.width() / 2 - measureRect.fLeft;
	tipLeftY = tipRectLeft.fTop + tipRectLeft.height() / 2 - measureRect.height() / 2 - measureRect.fTop;

	fontText->measureText(tipRight.c_str(), tipRight.size(), SkTextEncoding::kUTF8, &measureRect);
	tipRectRight.setXYWH(icon2Pos.fX - measureRect.width() / 2 - measureRect.fLeft,
		titleBar->dragRect.fBottom - measureRect.height() - iconSize,
		measureRect.width() + iconSize, measureRect.height() + iconSize);
	tipRightX = tipRectRight.fLeft + tipRectRight.width() / 2 - measureRect.width() / 2 - measureRect.fLeft;
	tipRightY = tipRectRight.fTop + tipRectRight.height() / 2 - measureRect.height() / 2 - measureRect.fTop;
}

void CalendarHeader::OnLeftBtnDown(const int& x, const int& y)
{
	if (mouseInLeft) {
		WsConn::Get()->PostMsg(R"({"msgName":"changePrevMonth"})");
	}
	if (mouseInRight) {
		WsConn::Get()->PostMsg(R"({"msgName":"changeNextMonth"})");
	}
}

void CalendarHeader::OnMouseMove(const int& x, const int& y)
{
	auto win = MainWin::Get();
	if (x<c1Center.fX - r || x>c2Center.fX + r || y>c1Center.fY - r || y<c1Center.fY + r){
		if (mouseInLeft || mouseInRight) {
			mouseInLeft = false;
			mouseInRight = false;
			MainWin::Cursor(IDC_ARROW);
			win->Refresh();
		}
	}
	SkPoint mousePos{ SkPoint::Make(x, y) };
	auto leftFlag = Util::IsInCircle(c1Center,r, mousePos);
	auto rightFlag = Util::IsInCircle(c2Center, r, mousePos);
	if (mouseInLeft && !leftFlag) {
		mouseInLeft = false;
		MainWin::Cursor(IDC_ARROW);
		win->Refresh();
		return;
	}
	if (mouseInRight && !rightFlag) {
		mouseInRight = false;
		MainWin::Cursor(IDC_ARROW);
		win->Refresh();
		return;
	}
	if (!mouseInLeft && leftFlag) {
		mouseInLeft = true;
		MainWin::Cursor(IDC_HAND);
		win->Refresh();
		return;
	}
	if (!mouseInRight && rightFlag) {
		mouseInRight = true;
		MainWin::Cursor(IDC_HAND);
		win->Refresh();
		return;
	}
}

void CalendarHeader::SetData(rapidjson::Value& data)
{
	yearMonthStr = data["activeDateMonth"].GetString();
	auto lang = data["lang"].GetObj();
	tipLeft = lang["prevMonth"].GetString();
	tipRight = lang["nextMonth"].GetString();
	measure();
}

void CalendarHeader::measure()
{
	auto win = MainWin::Get();
	auto fontText = Font::GetText();
	auto titleBar = TitleBar::Get();

	textSize = 28 * win->dpi;
	fontText->setSize(textSize);
	SkRect measureRect;
	fontText->measureText(yearMonthStr.c_str(), yearMonthStr.size(), SkTextEncoding::kUTF8, &measureRect);
	textPos.fX = win->w / 2 - measureRect.width() / 2 - measureRect.fLeft;	
	textPos.fY = titleBar->dragRect.fBottom + 6 * win->dpi + 32 * win->dpi / 2 - measureRect.height() / 2 - measureRect.fTop;
}
