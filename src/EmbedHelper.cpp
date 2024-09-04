#include <iostream>
#include <sstream>

#include "EmbedHelper.h"
#include "App.h"
#include "MainWin.h"

namespace {
    bool isEmbeded{ false };
    WNDPROC OldProc;
}

bool checkMouse(POINT& point) {
    POINT globalPos;
    GetCursorPos(&globalPos);
    auto win = App::GetWin();
    if (globalPos.x > win->x && globalPos.y > win->y && 
        globalPos.x < win->x + win->w && globalPos.y < win->y + win->h) {
        point.x = globalPos.x - win->x;
        point.y = globalPos.y - win->y;
    }
    else
    {
        if (win->isMouseDown) {
            win->isMouseDown = false;
        }
        return false;
    }
    HWND hwnd = WindowFromPoint(globalPos);
    WCHAR className[28];
    int len = GetClassName(hwnd, className, 28);
    if ((lstrcmp(TEXT("SysListView32"), className) == 0) ||
        (lstrcmp(TEXT("WorkerW"), className) == 0) ||
        (lstrcmp(TEXT("Progman"), className) == 0)) {
        return true;
    }
    return false;
}

RAWINPUT* getRawInput(HRAWINPUT lParam) {
    UINT dwSize = sizeof(RAWINPUT);
    static BYTE lpb[sizeof(RAWINPUT)];
    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
    return (RAWINPUT*)lpb;
}

void postMsg(UINT msg, WPARAM wParam, LPARAM lParam) {
    auto win = App::GetWin();
    PostMessage(win->hwnd, msg, wParam, lParam);
}
void postMsg(UINT msg, WPARAM wParam, const POINT& point) {
    auto win = App::GetWin();
    auto lParam = MAKELPARAM(point.x, point.y);
    PostMessage(win->hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK EmbedHelper::handleWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_INPUT && isEmbeded)
    {
        POINT point;
        if (!checkMouse(point)) return 0;
        auto raw = getRawInput((HRAWINPUT)lParam);
        switch (raw->header.dwType)
        {
            case RIM_TYPEMOUSE:
            {
                RAWMOUSE rawMouse = raw->data.mouse;
                if (rawMouse.usButtonFlags == RI_MOUSE_WHEEL)
                {
                    auto wheelDelta = (float)(short)rawMouse.usButtonData;
                    auto wParam = wheelDelta > 0 ? SB_LINEUP : SB_LINEDOWN;
                    postMsg(WM_VSCROLL, wParam, 0);
                    break;
                }                
                switch (rawMouse.ulButtons)
                {
                    case RI_MOUSE_LEFT_BUTTON_DOWN:
                    {
                        postMsg(WM_LBUTTONDOWN, MK_LBUTTON, point);
                        break;
                    }
                    case RI_MOUSE_LEFT_BUTTON_UP:
                    {
                        postMsg(WM_LBUTTONUP, MK_LBUTTON, point);
                        break;
                    }
                    default:
                    {
                        postMsg(WM_MOUSEMOVE, 0x0020, point);
                        return 0;
                    }
                }
                break;
            }
        }
    }
    return CallWindowProc(OldProc, hWnd, uMsg, wParam, lParam);
}
void EmbedHelper::roteInput()
{
    auto win = App::GetWin();
    RAWINPUTDEVICE rids[1];
    rids[0].usUsagePage = 0x01;
    rids[0].usUsage = 0x02;
    rids[0].dwFlags = 0x00000100;
    rids[0].hwndTarget = win->hwnd;
    RegisterRawInputDevices(rids, 1, sizeof(rids[0]));
    OldProc = (WNDPROC)SetWindowLongPtr(win->hwnd, GWLP_WNDPROC, (LONG_PTR)EmbedHelper::handleWindowMessage);
}
EmbedHelper::~EmbedHelper()
{
}

void EmbedHelper::Embed() { 
    auto win = App::GetWin();
    if (isEmbeded) {
        SetWindowLongPtr(win->hwnd, GWLP_WNDPROC, (LONG_PTR)OldProc);
        SetParent(win->hwnd, nullptr);
        isEmbeded = false;
        SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, nullptr, SPIF_UPDATEINIFILE);
    }
    else {
        HWND progman = FindWindow(L"Progman", NULL);
        SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, NULL);
        HWND workerW{ nullptr };
        EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
            HWND defView = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
            if (defView != NULL) {
                HWND* ww = (HWND*)lParam;
                *ww = FindWindowEx(NULL, hwnd, L"WorkerW", NULL);
            }
            return TRUE;
            }, (LPARAM)&workerW); 
        win->EnableAlpha(workerW);
        SetParent(win->hwnd, workerW);
        roteInput();
        isEmbeded = true;
    }
}

bool EmbedHelper::IsEmbed()
{
    return isEmbeded;
}
