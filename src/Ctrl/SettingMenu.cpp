
#include <stdlib.h>
#include "SettingMenu.h"
#include "TitleBar.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../TypeDefine.h"
#include "../Skin.h"
#include "../Util.h"
#include "../WsConn.h"
#include <ShlObj.h>

namespace {
	std::unique_ptr<SettingMenu> settingMenu;
}

void SettingMenu::Init()
{
	settingMenu = std::make_unique<SettingMenu>();
	auto win = MainWin::Get();
	win->paintHandlers.push_back(std::bind(&SettingMenu::OnPaint, settingMenu.get(), std::placeholders::_1));
	win->dpiHandlers.push_back(std::bind(&SettingMenu::OnDpi, settingMenu.get()));
	win->mouseMoveHandlers.push_back(std::bind(&SettingMenu::OnMouseMove, settingMenu.get(), std::placeholders::_1, std::placeholders::_2));
	win->leftBtnDownHandlers.push_back(std::bind(&SettingMenu::OnLeftBtnDown, settingMenu.get(), std::placeholders::_1, std::placeholders::_2));
}

SettingMenu* SettingMenu::Get()
{
	return settingMenu.get();
}

void SettingMenu::OnPaint(SkCanvas* canvas)
{
	if (!visible) return;
	auto skin = Skin::Get();
	SkPaint paint;
	paint.setAntiAlias(true);
	paint.setColor(skin->menuBg);
	auto rr = SkRRect::MakeRectXY(bg, radiu, radiu);
	canvas->drawRRect(rr, paint);
	
	if (hoverIndex != -1) {
		auto top{ bg.fTop + hoverIndex * itemHeight + margin };
		auto hoverBg = SkRect::MakeLTRB(bg.fLeft + margin, top, bg.fRight - margin, top+ itemHeight);
		paint.setColor(skin->menuHover);
		auto rr = SkRRect::MakeRectXY(hoverBg, radiu, radiu);
		canvas->drawRRect(rr, paint);
	}

	auto fontIcon = Font::GetIcon();
	fontIcon->setSize(fontSize);
	auto fontText = Font::GetText();
	fontText->setSize(fontSize);
	paint.setColor(skin->text1);
	for (size_t i = 0; i < 4; i++)
	{
		canvas->drawString(iconCode[i], bg.fLeft+ margin +textLeftMargin, textStartY + itemHeight*i, *fontIcon, paint);
		canvas->drawString(menuText[i].data(), bg.fLeft + margin *2 + textLeftMargin*3, textStartY + itemHeight * i, *fontText, paint);
	}
}

void SettingMenu::OnDpi()
{
	auto titleBar = TitleBar::Get();
	auto win = MainWin::Get();
	itemHeight = 32 * win->dpi;
	margin = 4 * win->dpi;
	textLeftMargin = 7 * win->dpi;
	fontSize = 14* win->dpi;
	bg = SkRect::MakeLTRB(titleBar->settingRect.fRight - 108 * win->dpi,
		titleBar->settingRect.fBottom + 4 * win->dpi,
		titleBar->settingRect.fRight,
		titleBar->settingRect.fBottom + 4 * win->dpi + 136 * win->dpi);
	radiu = 4 * win->dpi;
	textStartY = bg.fTop + 26 * win->dpi;
}

void SettingMenu::OnLeftBtnDown(const int& x, const int& y)
{
	if (!visible) return;
	if (hoverIndex == 0) {
		WsConn::Get()->PostMsg(R"({"msgName":"commonSet"})");
	}
	else if (hoverIndex == 1) {
		system("start https://docs.hikvision.com/#/file/nodcnpCtQNbetShKAgO3RUcrHnr");
	}
	else if (hoverIndex == 2) {
		system("start https://wj.hikvision.com.cn/wenjuan/#/answer-sheet?id=66dfa62380e065d458e2f400&isHeader=1&headerStyle=light");
	}
	else if (hoverIndex == 3) {
		auto win = MainWin::Get();
		win->Close();
		return;
	}
	visible = false;
	MainWin::Get()->Refresh();

}

void SettingMenu::OnMouseMove(const int& x, const int& y)
{
	if (!visible) return;
	auto flag = bg.contains(x, y);
	if (!flag && hoverIndex==-1) {
		return;
	}
	else if (!flag) {
		hoverIndex = -1;
		MainWin::Cursor(IDC_ARROW);
		MainWin::Get()->Refresh();
		return;
	}
	int index{ -1 };
	for (size_t i = 0; i < 4; i++)
	{
		if(y < bg.fTop + (i + 1)* itemHeight){
			index = i;
			break;
		}
	}
	if (index != hoverIndex) {
		hoverIndex = index;
		MainWin::Cursor(IDC_HAND);
		MainWin::Get()->Refresh();
	}
}

void SettingMenu::Show()
{
	visible = true;
	MainWin::Get()->Refresh();
}
