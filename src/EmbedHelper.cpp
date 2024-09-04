#include <iostream>
#include <sstream>

#include "EmbedHelper.h"
#include "App.h"
#include "MainWin.h"

namespace {
    bool isEmbeded{ false };
    WNDPROC OldProc;
    static HWND workerW{ nullptr };
}
POINT EmbedHelper::getMousePosInWin() {
    POINT point;
    GetCursorPos(&point);
    auto win = App::GetWin();    
    if (point.x > win->x && point.y > win->y && point.x < win->x+win->w && point.y < win->y+win->h) {
        point.x = point.x - win->x;
        point.y = point.y - win->y;
    }
    else
    {
        point.x = -100;
        point.y = -100;
    }
    return point;
}
bool EmbedHelper::isMouseOnDesktop() {
    POINT mousePos;
    GetCursorPos(&mousePos);
    HWND hwnd = WindowFromPoint(mousePos);
    WCHAR className[28];
    int len = GetClassName(hwnd, className, 28);
    if ((lstrcmp(TEXT("SysListView32"), className) == 0)||
        (lstrcmp(TEXT("WorkerW"), className) == 0) ||
        (lstrcmp(TEXT("Progman"), className) == 0)) {
        return true;
    }
    return false;
}
LRESULT CALLBACK EmbedHelper::handleWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (!isEmbeded) {
        return CallWindowProc(OldProc, hWnd, uMsg, wParam, lParam);
    }
    if (uMsg == WM_INPUT)
    {
        POINT point = getMousePosInWin();
        if (point.x < 0 && point.y < 0 && isEmbeded) {
            return 0;
        }
        if (!isMouseOnDesktop()) {
            return 0;
        }
        UINT dwSize = sizeof(RAWINPUT);
        static BYTE lpb[sizeof(RAWINPUT)];
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
        auto* raw = (RAWINPUT*)lpb;
        auto win = App::GetWin();
        switch (raw->header.dwType)
        {
            case RIM_TYPEMOUSE:
            {
                RAWMOUSE rawMouse = raw->data.mouse;
                if (rawMouse.usButtonFlags == RI_MOUSE_WHEEL)
                {
                    auto wheelDelta = (float)(short)rawMouse.usButtonData;
                    PostMessage(win->hwnd, WM_VSCROLL, wheelDelta > 0 ? SB_LINEUP : SB_LINEDOWN, 0);
                    break;
                }
                auto lParam = MAKELPARAM(point.x, point.y);
                switch (rawMouse.ulButtons)
                {
                    case RI_MOUSE_LEFT_BUTTON_DOWN:
                    {
                        PostMessage(win->hwnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
                        break;
                    }
                    case RI_MOUSE_LEFT_BUTTON_UP:
                    {
                        PostMessage(win->hwnd, WM_LBUTTONUP, MK_LBUTTON, lParam);
                        break;
                    }
                    default:
                    {
                        PostMessage(win->hwnd, WM_MOUSEMOVE, 0x0020, lParam);
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
        //std::string hwndStr = "0001015C"; 
        //std::stringstream ss;
        //ss << std::hex << hwndStr;
        //unsigned long hwndHex;
        //ss >> hwndHex; 
        //HWND hwnd = reinterpret_cast<HWND>(hwndHex);
        HWND progman = FindWindow(L"Progman", NULL);
        SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, NULL);
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

        //HDC hdcTarget = GetDC(workerW);
        //HDC hdcMemory = CreateCompatibleDC(hdcTarget);
        //HBITMAP hBitmap = CreateCompatibleBitmap(hdcTarget, win->w, win->h);
        //HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMemory, hBitmap); 
        //BITMAPINFO bmpInfo = { sizeof(BITMAPINFOHEADER), win->w, -win->h, 1, 32, BI_RGB, win->h * 4 * win->w, 0, 0, 0, 0 };
        //win->canvas->clear(0x88123456);
        //SetDIBits(hdcMemory, hBitmap, 0, win->h, &win->winPix.front(), &bmpInfo, DIB_RGB_COLORS);
        //BitBlt(hdcTarget, 0, 0, win->w, win->h, hdcMemory, 0, 0, SRCCOPY); 
        //SelectObject(hdcMemory, hOldBitmap);
        //DeleteObject(hBitmap);
        //DeleteDC(hdcMemory);
        //ReleaseDC(workerW, hdcTarget);
    }
}

bool EmbedHelper::IsEmbed()
{
    return isEmbeded;
}
