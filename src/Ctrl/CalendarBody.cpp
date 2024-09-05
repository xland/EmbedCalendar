#include "CalendarBody.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../TypeDefine.h"
#include "../Skin.h"
#include "WeekHeader.h"
namespace {
	std::unique_ptr<CalendarBody> calendarBody;
}

CalendarBody::CalendarBody()
{
}

CalendarBody::~CalendarBody()
{
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



	SkPoint pos{ SkPoint::Make(x,y) };
	for (size_t yIndex = 0; yIndex < 6; yIndex++) {
		pos.fY += 2 * win->dpi;
		for (size_t xIndex = 0; xIndex < 7; xIndex++)
		{
			paint.setColor(skin->hoverBg);
			canvas->drawCircle(pos.fX+r, pos.fY+r, r, paint);
			auto& item = items[yIndex * 7 + xIndex];

			paint.setColor(skin->text0);
			font->setSize(numSize);
			if (item.date.length() > 1) {
				canvas->drawString(item.date.data(), pos.fX + numX2, pos.fY + numY, *font, paint);
			}
			else {
				canvas->drawString(item.date.data(), pos.fX + numX1, pos.fY + numY, *font, paint);
			}
			

			pos.fX += span;
		}
		pos.fY += 2*r;
		pos.fX = x;
	}
}

void CalendarBody::OnDpi()
{
	auto win = MainWin::Get();
	r = 28 * win->dpi;
	x = 30 * win->dpi;
	y = WeekHeader::Get()->bottom + 6 * win->dpi;
	span = (win->w - 20 * win->dpi * 2) / 7;

	numSize = 20 * win->dpi;
	auto font = Font::GetText();
	font->setSize(numSize);
	SkRect measureRect;
	font->measureText("1", 1, SkTextEncoding::kUTF8, &measureRect);
	numX1 = r - measureRect.width() / 2 - measureRect.fLeft;
	numY = 0 - measureRect.fTop + 10*win->dpi;

	font->measureText("11", 2, SkTextEncoding::kUTF8, &measureRect);
	numX2 = r - measureRect.width() / 2 - measureRect.fLeft;

}

void CalendarBody::OnLeftBtnDown(const int& x, const int& y)
{

}

void CalendarBody::OnMouseMove(const int& x, const int& y)
{
}

void CalendarBody::SetText(std::vector<DateItem>&& param)
{
	items = std::move(param);
}
