#include "App.h"
#include "MainWin.h"
#include "Font.h"

namespace {
	std::unique_ptr<App> app;
}

App::App()
{
}

App::~App()
{
}

void App::Init(HINSTANCE instance, std::wstring&& cmd)
{
	app = std::make_unique<App>();
	app->instance = instance;
	Font::Init();
	app->win = std::make_unique<MainWin>();
	app->win->Init();
}
App* App::Get()
{
	return app.get();
}
MainWin* App::GetWin()
{
	return app->win.get();
}
void App::Cursor(LPWSTR id) {
	if (!app->win.get()) {
		return;
	}
	static auto _id{ IDC_NO };
	if (_id == id) {
		return;
	}
	_id = id;
	SetCursor(LoadCursor(nullptr, id));
}