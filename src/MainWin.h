#pragma once
#include <Windows.h>
#include <vector>
#include <functional>
#include "TypeDefine.h"


class MainWin
{
public:
	MainWin();
	~MainWin();
	void Init();
	void Refresh();
public:
	HWND hwnd;
	int x{ 100 }, y{ 100 }, w{ 580 }, h{580};
	bool isMouseDown{ false };
	float scaleFactor{ 1.0 };
private:
	void createWindow();
	void repaint();
	void getDpi();
	void initCanvas();
	void processNativeMsg(UINT msg, WPARAM wParam, LPARAM lParam);
	void onLeftBtnDown(const int& x, const int& y);
	void onLeftBtnUp(const int& x, const int& y);
	void onMouseMove(const int& x, const int& y);
	void onMouseDrag(const int& x, const int& y);
	void onCustomMsg(const EventType& type, const uint32_t& msg);
	static LRESULT CALLBACK routeWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	std::vector<MouseEventCB> mouseMoveHandlers;
	std::vector<MouseEventCB> mouseDragHandlers;
	std::vector<MouseEventCB> leftBtnDownHandlers;
	std::vector<MouseEventCB> leftBtnUpHandlers;
	std::vector<CustomEventCB> customEventHandlers;
	std::vector<PaintEventCB> paintHandlers;
	bool refreshFlag{ false };
	std::vector<SkColor> winPix;
	std::unique_ptr<SkCanvas> winCanvas;
};

