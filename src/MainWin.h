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
public:
	HWND hwnd;
	int x, y, w, h;
	bool isMouseDown{ false };
	float scaleFactor{ 1.0 };
private:
	void createWindow();
	LRESULT processNativeMsg(UINT msg, WPARAM wParam, LPARAM lParam);
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
};

