#pragma once
#include <Windows.h>
#include <vector>
#include <functional>


#include "TypeDefine.h"

class TitleBar;
class WsConn;
class Skin;
class CalendarHeader;
class MainWin
{
public:
	MainWin();
	~MainWin();
	static void Init(HINSTANCE instance, std::wstring&& cmd);
	static MainWin* Get();
	static void Cursor(LPWSTR id);
	void Refresh();
	void Close();
public:
	HINSTANCE instance;
	std::unique_ptr<SkCanvas> canvas;
	std::vector<SkColor> winPix;
	HWND hwnd{nullptr};
	int x{ 100 }, y{ 100 }, w{ 580 }, h{580};
	bool isMouseDown{ false };
	float dpi{ 1.0 };
public:
	std::vector<MouseEventCB> mouseMoveHandlers;
	std::vector<MouseEventCB> mouseDragHandlers;
	std::vector<MouseEventCB> leftBtnDownHandlers;
	std::vector<MouseEventCB> leftBtnUpHandlers;
	std::vector<CustomEventCB> customEventHandlers;
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
	void onMouseDrag(const int& x, const int& y);
	void onDpiChange();
	void onCustomMsg(const EventType& type, const uint32_t& msg);
	static LRESULT CALLBACK routeWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	bool refreshFlag{ false };
	bool isTrackMouseEvent{ false };
};

