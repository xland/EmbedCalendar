#pragma once
#include <Windows.h>
#include <vector>
#include <functional>
#include <rapidjson/document.h>

#include "TypeDefine.h"

class TitleBar;
class WsConn;
class Skin;
class CalendarHeader;
class MainWin
{
public:
	MainWin(HINSTANCE instance);
	~MainWin() = default;
	static void Init(HINSTANCE instance, std::wstring&& cmd);
	static MainWin* Get();
	static void Dispose();
	static void Cursor(LPWSTR id);
	void Refresh();
	void Close();
	void ShowList();
	void HideList();
public:
	HINSTANCE instance;
	std::unique_ptr<SkCanvas> canvas{nullptr};
	std::vector<SkColor> winPix;
	HWND hwnd{nullptr};
	int x{ 100 }, y{ 100 }, w{ 580 }, h{580};
	float dpi{ -1.0 };
	bool stopEventFlag{ false };
public:
	std::vector<MouseEventCB> mouseMoveHandlers;
	std::vector<WheelEventCB> mouseWheelHandlers;
	std::vector<MouseEventCB> leftBtnDownHandlers;
	std::vector<MouseEventCB> leftBtnUpHandlers;
	std::vector<PaintEventCB> paintHandlers;
	std::vector<DpiEventCB> dpiHandlers;
private:
	void repaint();
	void createWindow();
	void getDpi();
	void initCanvas();
	LRESULT processNativeMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void onLeftBtnDown(const int& x, const int& y);
	void onLeftBtnUp(const int& x, const int& y);
	void onMouseMove(const int& x, const int& y);
	void onMouseWheel(const int& span);
	void onDpiChange();
	void onDataReady(rapidjson::Document* d);
	static LRESULT CALLBACK routeWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	bool dataReady{ false };
	bool refreshFlag{ false };
	bool isTrackMouseEvent{ false };
};

