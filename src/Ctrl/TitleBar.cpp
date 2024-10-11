#include <functional>
#include <include/core/SkPaint.h>
#include <include/core/SkRRect.h>

#include "TitleBar.h"
#include "SettingMenu.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../TypeDefine.h"
#include "../Skin.h"

namespace {
	std::unique_ptr<TitleBar> titleBar;
}

void TitleBar::Init()
{
	titleBar = std::make_unique<TitleBar>();
	auto win = MainWin::Get();
	win->paintHandlers.push_back(std::bind(&TitleBar::OnPaint, titleBar.get(), std::placeholders::_1));
	win->dpiHandlers.push_back(std::bind(&TitleBar::OnDpi, titleBar.get()));
	win->mouseMoveHandlers.push_back(std::bind(&TitleBar::OnMouseMove, titleBar.get(), std::placeholders::_1, std::placeholders::_2));
	win->leftBtnDownHandlers.push_back(std::bind(&TitleBar::OnLeftBtnDown, titleBar.get(), std::placeholders::_1, std::placeholders::_2));
}

TitleBar* TitleBar::Get()
{
	return titleBar.get();
}

bool TitleBar::IsInCaption(const int& x, const int& y)
{
	if (dragRect.contains(x, y)) {
		return true;
	}
	return false;
}

void TitleBar::OnPaint(SkCanvas* canvas)
{
	SkPaint paint;
	paint.setAntiAlias(true);
	auto skin = Skin::Get();
	auto win = MainWin::Get();
	if (mouseInPinBtn) {
		paint.setColor(skin->hoverBg);
		auto win = MainWin::Get();
		auto rr = SkRRect::MakeRectXY(pinRect, 2 * win->dpi, 2 * win->dpi);
		canvas->drawRRect(rr, paint);
	}
	if (mouseInSettingBtn) {
		paint.setColor(skin->hoverBg);
		auto rr = SkRRect::MakeRectXY(settingRect, 2 * win->dpi, 2 * win->dpi);
		canvas->drawRRect(rr, paint);
	}
	auto font = Font::GetIcon();
	font->setSize(fontSize);
	paint.setColor(skin->text1);
	canvas->drawString(settingIcon, settingPos.fX, settingPos.fY, *font, paint);
	canvas->drawString(pinIcon, pinPos.fX, pinPos.fY, *font, paint);
	if (mouseInPinBtn) {
		paint.setColor(skin->toolTipBg);
		auto rr = SkRRect::MakeRectXY(tipRectUnEmbed, 4 * win->dpi, 4 * win->dpi);
		canvas->drawRRect(rr, paint);
		auto fontText = Font::GetText();
		fontText->setSize(tipSize);
		paint.setColor(skin->toolTipText);
		canvas->drawString(tipUnEmbed.c_str(), tipUnEmbedX, tipUnEmbedY, *fontText, paint);
	}
}

void TitleBar::OnDpi()
{
	auto win = MainWin::Get();
	fontSize = 18 * win->dpi;
	auto margin = 12 * win->dpi;
	auto size = 28 * win->dpi;
	tipSize = 14* win->dpi;
	settingRect.setXYWH(win->w - margin - size, margin, size, size);
	pinRect.setXYWH(settingRect.fLeft-margin-size, margin, size, size);	
	dragRect.setLTRB(0, 0, pinRect.fLeft - margin, 48 * win->dpi);

	auto font = Font::GetIcon();
	font->setSize(fontSize);
	SkRect measureRect;
	int length = std::mbstowcs(nullptr, settingIcon, 0); //3
	font->measureText(settingIcon, length, SkTextEncoding::kUTF8, &measureRect);
	settingPos.fX = settingRect.centerX() - measureRect.width() / 2 - measureRect.fLeft;
	settingPos.fY = settingRect.centerY() - measureRect.height() / 2 - measureRect.fTop;
	pinPos.fX = settingPos.fX - (settingRect.fLeft - pinRect.fLeft);
	pinPos.fY = settingPos.fY;

	auto fontText = Font::GetText();
	fontText->setSize(tipSize);
	fontText->measureText(tipEmbed.c_str(), tipEmbed.size(), SkTextEncoding::kUTF8, &measureRect);
	tipRectEmbed.setXYWH(pinPos.fX - measureRect.width() / 2 - measureRect.fLeft,
		pinRect.fBottom+3*win->dpi,
		measureRect.width() + tipSize,
		measureRect.height() + tipSize);
	tipEmbedX = tipRectEmbed.fLeft + tipRectEmbed.width() / 2 - measureRect.width() / 2 - measureRect.fLeft;
	tipEmbedY = tipRectEmbed.fTop + tipRectEmbed.height() / 2 - measureRect.height() / 2 - measureRect.fTop;

	fontText->measureText(tipUnEmbed.c_str(), tipUnEmbed.size(), SkTextEncoding::kUTF8, &measureRect);
	tipRectUnEmbed.setXYWH(pinPos.fX - measureRect.width() / 2 - measureRect.fLeft,
		pinRect.fBottom + 3 * win->dpi, measureRect.width() + tipSize,
		measureRect.height() + tipSize);
	tipUnEmbedX = tipRectUnEmbed.fLeft + tipRectUnEmbed.width() / 2 - measureRect.width() / 2 - measureRect.fLeft;
	tipUnEmbedY = tipRectUnEmbed.fTop + tipRectUnEmbed.height() / 2 - measureRect.height() / 2 - measureRect.fTop;

}

void TitleBar::OnLeftBtnDown(const int& x, const int& y)
{
	if (mouseInPinBtn) {
		mouseInPinBtn = false;
		auto win = MainWin::Get();
		SendMessage(win->hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(x, y));
	}
	else if (mouseInSettingBtn) {
		auto win = MainWin::Get();
		auto settingMenu = SettingMenu::Get();
		if (!settingMenu->visible) {
			win->stopEventFlag = true;
			SettingMenu::Get()->Show();
		}
	}
}

void TitleBar::OnMouseMove(const int& x, const int& y)
{
	auto pinFlag = pinRect.contains(x, y);
	auto settingFlag = settingRect.contains(x, y);
	auto win = MainWin::Get();
	if (mouseInPinBtn && !pinFlag) {
		mouseInPinBtn = false;
		MainWin::Cursor(IDC_ARROW);
		win->Refresh();
		return;
	}
	if (mouseInSettingBtn && !settingFlag) {
		mouseInSettingBtn = false;
		MainWin::Cursor(IDC_ARROW);
		win->Refresh();
		return;
	}
	if (!mouseInPinBtn && pinFlag) {
		mouseInPinBtn = true;
		MainWin::Cursor(IDC_HAND);
		win->Refresh();
		return;
	}
	if (!mouseInSettingBtn && settingFlag) {
		mouseInSettingBtn = true;
		MainWin::Cursor(IDC_HAND);
		win->Refresh();
		return;
	}
}

void TitleBar::SetData(rapidjson::Value& data)
{
	auto lang = data["lang"].GetObj();
	tipEmbed = lang["embed"].GetString();
	tipUnEmbed = lang["unEmbed"].GetString();
}
