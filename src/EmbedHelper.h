#pragma once
#include <Windows.h> 

class EmbedHelper
{
public:
	explicit EmbedHelper(){}
	~EmbedHelper();
	static void Embed();
	static bool IsEmbed();
private:
	static void roteInput();
	static LRESULT CALLBACK handleWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static bool isMouseOnDesktop();
	static POINT getMousePosInWin();
};