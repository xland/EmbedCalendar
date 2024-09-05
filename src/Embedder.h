#pragma once
#include <include/core/SkColor.h>
#include <Windows.h>

class Embedder
{
public:
	Embedder();
	~Embedder();
	static void Init();
	static Embedder* Get();
	void Embed();
	void UnEmbed();
	void TimerCB();
	void RefreshDesktop();
public:
	bool isEmbedded{false};
	bool isColorWallPaper{false};
	SkColor wallPaperColor;
private:
	void findWorkerW();
	void checkIsColorWallPaper();
	void getWallPaperColor();
	void roteInput();
	static void postMsg(UINT msg, WPARAM wParam, LPARAM lParam);
	static void postMsg(UINT msg, WPARAM wParam, const POINT& point);
	static bool checkMouseMsg(POINT& point);
	static RAWINPUT* getRawInput(HRAWINPUT lParam);
	static LRESULT CALLBACK processMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	HWND workerW{nullptr};
};

