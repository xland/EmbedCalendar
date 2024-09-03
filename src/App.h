#pragma once
#include <Windows.h>
#include <string>
#include <memory>

class MainWin;
class App
{
public:
	App();
	~App();
	static App* Get();
	static void Init(HINSTANCE instance, std::wstring&& cmd);
	static void Cursor(LPWSTR id);
public:
	HINSTANCE instance;
private:
private:
	std::unique_ptr<MainWin> win;

};