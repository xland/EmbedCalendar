#include "WeekHeader.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../TypeDefine.h"
#include "../Skin.h"
#include "CalendarHeader.h"

namespace {
	std::unique_ptr<WeekHeader> weekHeader;
}

void WeekHeader::Init()
{
	weekHeader = std::make_unique<WeekHeader>();
	auto win = MainWin::Get();
	win->paintHandlers.push_back(std::bind(&WeekHeader::OnPaint, weekHeader.get(), std::placeholders::_1));
	win->dpiHandlers.push_back(std::bind(&WeekHeader::OnDpi, weekHeader.get()));
}

WeekHeader* WeekHeader::Get()
{
	return weekHeader.get();
}

void WeekHeader::OnPaint(SkCanvas* canvas)
{
	auto font = Font::GetText();
	auto skin = Skin::Get();
	SkPaint paint;
	font->setSize(fontSize);
	paint.setColor(skin->text1);
	for (size_t i = 0; i < textArr.size(); i++)
	{
		canvas->drawString(textArr[i].data(), xPosArr[i], yPos, *font, paint);
	}
}
	

void WeekHeader::OnDpi()
{
	auto win = MainWin::Get();
	fontSize = 20 * win->dpi;
	auto font = Font::GetText();
	auto calendarHeader = CalendarHeader::Get();
	auto left = 20 * win->dpi;
	auto right = win->w - left;
	auto top = 26 * win->dpi + calendarHeader->c1Center.fY + calendarHeader->r;
	bottom = top + 28 * win->dpi;


	font->setSize(fontSize);
	SkRect measureRect;
	font->measureText(textArr[0].c_str(), textArr[0].size(), SkTextEncoding::kUTF8, &measureRect);

	auto span = (right - left) / 7;
	xPosArr[0] = left + span / 2 - measureRect.width() / 2 - measureRect.fLeft;
	xPosArr[1] = xPosArr[0] + span;
	xPosArr[2] = xPosArr[1] + span;
	xPosArr[3] = xPosArr[2] + span;
	xPosArr[4] = xPosArr[3] + span;
	xPosArr[5] = xPosArr[4] + span;
	xPosArr[6] = xPosArr[5] + span;
	yPos = top + 28 / 2 - measureRect.height() / 2 - measureRect.fTop;
}

void WeekHeader::SetData(rapidjson::Value& data)
{
	auto arr = data["weekLables"].GetArray();
	textArr.clear();
	for (size_t i = 0; i < arr.Size(); i++)
	{
		textArr.push_back(arr[i].GetString());
	}
}

