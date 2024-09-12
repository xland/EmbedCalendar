#include <Windows.h>

#include "MainWin.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
    //MainWin::Init(hInstance, std::wstring(lpCmdLine));
    MainWin::Init(hInstance, std::wstring(L"EmbedCalendar_61860"));
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    MainWin::Dispose();
    return 0;
}