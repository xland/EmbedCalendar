#pragma once
#include <Windows.h>
#include <vector>
#include <functional>
#include <d2d1.h>
#include <dwmapi.h>
#include <versionhelpers.h>

#include "TypeDefine.h"

class TitleBar;
class MainWin
{
public:
	MainWin();
	~MainWin();
	void Init();
	void Refresh();
	void Close();
	bool EnableAlpha(HWND hwnd);
public:
	std::unique_ptr<SkCanvas> canvas;
	std::vector<SkColor> winPix;
	HWND hwnd;
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
	std::unique_ptr<TitleBar> titleBar;
	ID2D1Factory* sg_pID2D1Factory;
	ID2D1HwndRenderTarget* renderTarget;
	ID2D1Bitmap* bitmap;
};

