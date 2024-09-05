#pragma once
#include <Windows.h> 
#include <include/core/SkColor.h>

class EmbedHelper
{
public:
	explicit EmbedHelper(){}
	~EmbedHelper();
	static void Embed();
	static bool IsEmbed();
	static bool IsPureColor();
	static SkColor GetPureColor();
private:
	static void roteInput();
	static LRESULT CALLBACK handleWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};