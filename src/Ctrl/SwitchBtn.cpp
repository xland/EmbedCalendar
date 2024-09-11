#include "SwitchBtn.h"
#include "CalendarBody.h"
#include "../WsConn.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../TypeDefine.h"
#include "../Skin.h"
#include "../Util.h"
#include "../Embedder.h"

namespace {
	std::unique_ptr<SwitchBtn> switchBtn;
}

void SwitchBtn::Init()
{
	switchBtn = std::make_unique<SwitchBtn>();
	auto win = MainWin::Get();
	win->paintHandlers.push_back(std::bind(&SwitchBtn::OnPaint, switchBtn.get(), std::placeholders::_1));
	win->dpiHandlers.push_back(std::bind(&SwitchBtn::OnDpi, switchBtn.get()));
	win->mouseMoveHandlers.push_back(std::bind(&SwitchBtn::OnMouseMove, switchBtn.get(), std::placeholders::_1, std::placeholders::_2));
	win->leftBtnDownHandlers.push_back(std::bind(&SwitchBtn::OnLeftBtnDown, switchBtn.get(), std::placeholders::_1, std::placeholders::_2));
}

SwitchBtn* SwitchBtn::Get()
{
	return switchBtn.get();
}

void SwitchBtn::OnPaint(SkCanvas* canvas)
{
	auto fontText = Font::GetText();
	auto fontIcon = Font::GetIcon();
	fontText->setSize(fontSize);
	fontIcon->setSize(fontSize);
	SkPaint paint;
	//if (isMouseIn) {
	//	paint.setColor(Skin::Get()->hoverBg);
	//	canvas->drawRect(rect,paint);
	//}
	paint.setColor(0xFF007AFF);
	if (listVisible) {
		canvas->drawString(textShow.data(), textX, textY, *fontText, paint);
		const char* iconCode{ (const char*)u8"\ue708" };
		canvas->drawString(iconCode, iconX, textY, *fontIcon, paint);
	}
	else {
		canvas->drawString(textHide.data(), textX, textY, *fontText, paint);
		const char* iconCode{ (const char*)u8"\ue70f" };
		canvas->drawString(iconCode, iconX, textY, *fontIcon, paint);
	}
}

void SwitchBtn::OnDpi()
{
	auto win = MainWin::Get();
	fontSize = 20 * win->dpi;
	auto r = win->w - 32 * win->dpi;
	auto b = win->h - 20 * win->dpi;
	rect.setLTRB(r-120* win->dpi, b-42* win->dpi, r, b);

	textX = rect.fLeft+8*win->dpi;
	textY = rect.fTop + 28 * win->dpi;
	iconX = textX + 86 * win->dpi;
}

void SwitchBtn::OnLeftBtnDown(const int& x, const int& y)
{
	if (!isMouseIn) return;
	isMouseIn = false;
	auto win = MainWin::Get();
	if (listVisible) {
		win->HideList();
		OnDpi();
		listVisible = false;
		WsConn::Get()->PostMsg(R"({"msgName":"displayScheduleList","data":false})");
	}
	else {
		win->ShowList();
		OnDpi();
		listVisible = true;
		MainWin::Cursor(IDC_ARROW);
		WsConn::Get()->PostMsg(R"({"msgName":"displayScheduleList","data":true})");
	}
	Util::RefreshDesktop();
}

void SwitchBtn::OnMouseMove(const int& x, const int& y)
{
	auto flag = rect.contains(x, y);
	auto win = MainWin::Get();
	if (!flag && isMouseIn) {
		isMouseIn = false;
		MainWin::Cursor(IDC_ARROW);
		win->Refresh();
		return;
	}
	if (flag && !isMouseIn) {
		isMouseIn = true;
		MainWin::Cursor(IDC_HAND);
		win->Refresh();
	}
}

void SwitchBtn::SetData(rapidjson::Value& data)
{
	listVisible = data["displayScheduleList"].GetBool();
	auto lang = data["lang"].GetObj();
	textShow = lang["displaySchedule"].GetString();
	textHide = lang["hideSchedule"].GetString();
	if (listVisible) {
		MainWin::Get()->ShowList();
	}
}
