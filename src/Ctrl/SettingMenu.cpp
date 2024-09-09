

#include "SettingMenu.h"
#include "TitleBar.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../TypeDefine.h"
#include "../Skin.h"
#include "../Util.h"

namespace {
	std::unique_ptr<SettingMenu> settingMenu;
}

SettingMenu::SettingMenu()
{
}

SettingMenu::~SettingMenu()
{
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
	SkPaint paint;
	paint.setAntiAlias(true);
	paint.setColor(Skin::Get()->menuBg);	
	canvas->drawRRect(bg, paint);
	
	auto fontIcon = Font::GetIcon();
	fontIcon->setSize(fontSize);
	auto fontText = Font::GetText();
	fontText->setSize(fontSize);
	paint.setColor(Skin::Get()->text1);
	for (size_t i = 0; i < 4; i++)
	{
		canvas->drawString(iconCode[i], bg.rect().fLeft+leftMargin+textLeftMargin, textStartY + textHeight*i, *fontIcon, paint);
		canvas->drawString(menuText[i].data(), bg.rect().fLeft + leftMargin*2 + textLeftMargin*2, textStartY + textHeight * i, *fontText, paint);
	}
}

void SettingMenu::OnDpi()
{
	auto titleBar = TitleBar::Get();
	auto win = MainWin::Get();
	textHeight = 32 * win->dpi;
	leftMargin = 4 * win->dpi;
	textLeftMargin = 12 * win->dpi;
	fontSize = 14* win->dpi;
	auto rect = SkRect::MakeLTRB(titleBar->settingRect.fRight - 108 * win->dpi,
		titleBar->settingRect.fBottom + 4 * win->dpi,
		titleBar->settingRect.fRight,
		titleBar->settingRect.fBottom + 4 * win->dpi + 136 * win->dpi);
	auto r = 4 * win->dpi;
	SkVector radii[4]{ {r, r},{r, r}, //左上角 右上角
			{r, r}, {r, r}  //右下角 左下角
	};
	bg.setRectRadii(rect, radii);
	textStartY = rect.fTop + 26 * win->dpi;
}

void SettingMenu::OnLeftBtnDown(const int& x, const int& y)
{
}

void SettingMenu::OnMouseMove(const int& x, const int& y)
{
}
