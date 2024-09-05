

#include "CalendarHeader.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../TypeDefine.h"
#include "../Skin.h"
#include "TitleBar.h"

namespace {
	std::unique_ptr<CalendarHeader> calendarHeader;
}

CalendarHeader::CalendarHeader()
{
}

CalendarHeader::~CalendarHeader()
{
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
	auto font = Font::GetText();
	auto fontIcon = Font::GetIcon();
	auto skin = Skin::Get();
	SkPaint paint;
	paint.setAntiAlias(true);
	font->setSize(textSize);
	paint.setColor(skin->text0);
	canvas->drawString(yearMonthStr.c_str(), textPos.fX, textPos.fY, *font, paint);
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
}

void CalendarHeader::OnDpi()
{
	measure();
}

void CalendarHeader::OnLeftBtnDown(const int& x, const int& y)
{
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
	auto leftFlag = isInCircle(x, y,c1Center);
	auto rightFlag = isInCircle(x, y, c2Center);
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

void CalendarHeader::SetText(std::string&& text)
{
	yearMonthStr = text;
	measure();
}

void CalendarHeader::measure()
{
	auto win = MainWin::Get();
	auto font = Font::GetText();
	auto fontIcon = Font::GetIcon();
	textSize = 28 * win->dpi;
	iconSize = 14 * win->dpi;
	r = 16 * win->dpi;

	font->setSize(textSize);
	SkRect measureRect;
	font->measureText(yearMonthStr.c_str(), yearMonthStr.size(), SkTextEncoding::kUTF8, &measureRect);
	textPos.fX = win->w / 2 - measureRect.width() / 2 - measureRect.fLeft;
	c1Center.fY = TitleBar::Get()->dragRect.fBottom + 6 * win->dpi + 32 * win->dpi / 2 ;
	c2Center.fY = c1Center.fY;
	textPos.fY = c1Center.fY - measureRect.height() / 2 - measureRect.fTop;
	
	c1Center.fX = textPos.fX - 28 * win->dpi - r;
	c2Center.fX = textPos.fX + measureRect.width() + 28 * win->dpi + r;

	fontIcon->setSize(iconSize);
	int length = std::mbstowcs(nullptr, leftIcon, 0);
	fontIcon->measureText(leftIcon, length, SkTextEncoding::kUTF8, &measureRect);

	icon1Pos.fX = c1Center.fX - r + r - measureRect.width() / 2 - measureRect.fLeft;
	icon1Pos.fY = c2Center.fY - r + r - measureRect.height() / 2 - measureRect.fTop;

	icon2Pos.fY = icon1Pos.fY;
	icon2Pos.fX = c2Center.fX - (c1Center.fX - icon1Pos.fX);
}
bool CalendarHeader::isInCircle(float px, float py, const SkPoint& center) {
	float dx = px - center.fX;
	float dy = py - center.fY;
	float distance = std::sqrt(dx * dx + dy * dy);
	return distance <= r;
}