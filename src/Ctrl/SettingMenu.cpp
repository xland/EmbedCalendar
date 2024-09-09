#include "SettingMenu.h"
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
}

void SettingMenu::OnDpi()
{
}

void SettingMenu::OnLeftBtnDown(const int& x, const int& y)
{
}

void SettingMenu::OnMouseMove(const int& x, const int& y)
{
}
