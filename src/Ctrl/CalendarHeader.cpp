

#include "CalendarHeader.h"
#include "../App.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../TypeDefine.h"
#include "../EmbedHelper.h"
#include "../Skin.h"


CalendarHeader::CalendarHeader()
{
}

CalendarHeader::~CalendarHeader()
{
}

void CalendarHeader::Init()
{
	auto win = App::GetWin();
	win->paintHandlers.push_back(std::bind(&CalendarHeader::OnPaint, this, std::placeholders::_1));
	win->dpiHandlers.push_back(std::bind(&CalendarHeader::OnDpi, this));
	win->mouseMoveHandlers.push_back(std::bind(&CalendarHeader::OnMouseMove, this, std::placeholders::_1, std::placeholders::_2));
	win->leftBtnDownHandlers.push_back(std::bind(&CalendarHeader::OnLeftBtnDown, this, std::placeholders::_1, std::placeholders::_2));
}

void CalendarHeader::OnPaint(SkCanvas* canvas)
{
	Font::GetText();
	//canvas->drawString(yearMonthStr.c_str(), 20, 180, font, paint);
}

void CalendarHeader::OnDpi()
{
}

void CalendarHeader::OnLeftBtnDown(const int& x, const int& y)
{
}

void CalendarHeader::OnMouseMove(const int& x, const int& y)
{
}
