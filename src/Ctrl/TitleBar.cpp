#include <functional>
#include <include/core/SkPaint.h>

#include "TitleBar.h"
#include "../App.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../TypeDefine.h"
#include "../EmbedHelper.h"


TitleBar::TitleBar()
{
}

TitleBar::~TitleBar()
{
}

bool TitleBar::IsInCaption(const int& x, const int& y)
{
	if (dragRect.contains(x, y)) {
		return true;
	}
	return false;
}

void TitleBar::Init()
{
	auto win = App::GetWin();
	win->paintHandlers.push_back(std::bind(&TitleBar::OnPaint, this, std::placeholders::_1));
	win->dpiHandlers.push_back(std::bind(&TitleBar::OnDpi, this));
	win->mouseMoveHandlers.push_back(std::bind(&TitleBar::OnMouseMove, this, std::placeholders::_1, std::placeholders::_2));
	win->leftBtnDownHandlers.push_back(std::bind(&TitleBar::OnLeftBtnDown, this, std::placeholders::_1, std::placeholders::_2));
}

void TitleBar::OnPaint(SkCanvas* canvas)
{
	SkPaint paint;
	if (mouseInPinBtn) {
		paint.setColor(0x28000000);
		canvas->drawRect(pinRect, paint);
	}
	if (mouseInSettingBtn) {
		paint.setColor(0x28000000);
		canvas->drawRect(settingRect, paint);
	}
	auto font = Font::GetIcon();
	font->setSize(fontSize);
	paint.setColor(0xFFffffff);
	auto setting = (const char*)u8"\ue6e8";
	auto pin = (const char*)u8"\ue70c";
	SkRect measureRect;
	int length = std::mbstowcs(nullptr, setting, 0);
	font->measureText(setting, 3, SkTextEncoding::kUTF8, &measureRect);
	auto x = settingRect.centerX() - measureRect.width() / 2 - measureRect.fLeft;
	auto y = settingRect.centerY() - measureRect.height() / 2 - measureRect.fTop;
	canvas->drawString(setting, x, y, *font, paint);
	canvas->drawString(pin, x-(settingRect.fLeft-pinRect.fLeft), y, *font, paint);
}

void TitleBar::OnDpi()
{
	auto win = App::GetWin();
	auto margin = 10 * win->dpi;
	auto size = 28 * win->dpi;
	settingRect.setXYWH(win->w - margin - size, margin, size, size);
	pinRect.setXYWH(settingRect.fLeft-margin-size, margin, size, size);
	fontSize = 14 * win->dpi;
	dragRect.setLTRB(0, 0, pinRect.fLeft - margin, 48 * win->dpi);
}

void TitleBar::OnLeftBtnDown(const int& x, const int& y)
{
	if (mouseInPinBtn) {
		EmbedHelper::Embed();
		auto win = App::GetWin();
		SendMessage(win->hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(x, y));
	}
	else if (mouseInSettingBtn) {
		auto win = App::GetWin();
		win->Close();
	}
}

void TitleBar::OnMouseMove(const int& x, const int& y)
{
	auto pinFlag = pinRect.contains(x, y);
	auto settingFlag = settingRect.contains(x, y);
	auto win = App::GetWin();
	if (mouseInPinBtn && !pinFlag) {
		mouseInPinBtn = false;
		App::Cursor(IDC_ARROW);
		win->Refresh();
		return;
	}
	if (mouseInSettingBtn && !settingFlag) {
		mouseInSettingBtn = false;
		App::Cursor(IDC_ARROW);
		win->Refresh();
		return;
	}
	if (!mouseInPinBtn && pinFlag) {
		mouseInPinBtn = true;
		App::Cursor(IDC_HAND);
		win->Refresh();
		return;
	}
	if (!mouseInSettingBtn && settingFlag) {
		mouseInSettingBtn = true;
		App::Cursor(IDC_HAND);
		win->Refresh();
		return;
	}
}
