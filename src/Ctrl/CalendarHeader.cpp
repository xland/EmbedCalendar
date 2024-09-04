

#include "CalendarHeader.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../TypeDefine.h"
#include "../EmbedHelper.h"
#include "../Skin.h"

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
