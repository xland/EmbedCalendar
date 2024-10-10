#include <format>
#include <string>

#include "Embedder.h"
#include "MainWin.h"
#include "TypeDefine.h"
#include "Util.h"
#include "WsConn.h"

namespace {
	std::unique_ptr<Embedder> embedder;
    WNDPROC oldProc;
}

Embedder::Embedder()
{
}

Embedder::~Embedder()
{
}

void Embedder::Init()
{
	embedder = std::make_unique<Embedder>();
}

Embedder* Embedder::Get()
{
	return embedder.get();
}

void Embedder::Embed()
{
    findWorkerW();
    checkIsColorWallPaper();
    if (isColorWallPaper) {
        getWallPaperColor();
    }
    else {
        Util::EnableAlpha(workerW);
    }
    auto win = MainWin::Get();
    SetParent(win->hwnd, workerW);
    roteInput();
    SetTimer(win->hwnd, CheckWallPaperTimerId, 2000, NULL);
    auto msg = std::format(R"({{"msgName":"embedWin","data":{{"hasEmbed":true,"x":{},"y":{}}}}})",std::to_string(win->x), std::to_string(win->y));
    WsConn::Get()->PostMsg(std::move(msg));
    isEmbedded = true;
}

void Embedder::UnEmbed()
{
    auto win = MainWin::Get();
    KillTimer(win->hwnd, CheckWallPaperTimerId);
    SetWindowLongPtr(win->hwnd, GWLP_WNDPROC, (LONG_PTR)oldProc);
    SetParent(win->hwnd, nullptr);
    Util::DisableAlpha(workerW);
    Util::RefreshDesktop();
    WsConn::Get()->PostMsg(R"({"msgName":"embedWin","data":{"hasEmbed":false}})");
    isEmbedded = false;
}

void Embedder::TimerCB()
{
    //Util::DisableAlpha(workerW);

    bool flag = isColorWallPaper;
    checkIsColorWallPaper();
    if (!flag && isColorWallPaper) {
        Util::DisableAlpha(workerW);
        MainWin::Get()->Refresh();
        Util::RefreshDesktop();
        return;
    }
    if (flag && !isColorWallPaper) {
        Util::EnableAlpha(workerW);
        MainWin::Get()->Refresh();
        Util::RefreshDesktop();
        return;
    }
    if (isColorWallPaper) {
        auto color = wallPaperColor;
        getWallPaperColor();
        if (color != wallPaperColor) {
            MainWin::Get()->Refresh();
            return;
        }
    }
}

void Embedder::findWorkerW()
{
    if (workerW) return;
    HWND progman = FindWindow(L"Progman", NULL);
    //SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, NULL);
    SendMessage(progman, 0x052C, 0xD, 0);
    SendMessage(progman, 0x052C, 0xD, 1);
    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        HWND defView = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
        if (defView != NULL) {
            auto self = (Embedder*)lParam;
            self->workerW = FindWindowEx(NULL, hwnd, L"WorkerW", NULL);
        }
        return TRUE;
        }, (LPARAM)this);
}

void Embedder::checkIsColorWallPaper()
{
    std::wstring wallpaperPath(MAX_PATH, L'\0');
    SystemParametersInfoW(SPI_GETDESKWALLPAPER, MAX_PATH, &wallpaperPath.front(), 0);
    wallpaperPath.resize(wcslen(wallpaperPath.c_str()));
    isColorWallPaper = wallpaperPath.empty();
}

void Embedder::getWallPaperColor()
{
    HDC hdc = GetDC(workerW);
    COLORREF color = GetPixel(hdc, 6, 6);
    auto r = (unsigned)GetRValue(color);
    auto g = (unsigned)GetGValue(color);
    auto b = (unsigned)GetBValue(color);
    ReleaseDC(workerW, hdc);
    wallPaperColor = SkColorSetARGB(255, r, g, b);
}

bool Embedder::checkMouseMsg(POINT& point) {
    POINT globalPos;
    GetCursorPos(&globalPos);
    auto win = MainWin::Get();
    if (globalPos.x > win->x && globalPos.y > win->y &&
        globalPos.x < win->x + win->w && globalPos.y < win->y + win->h) {
        point.x = globalPos.x - win->x;
        point.y = globalPos.y - win->y;
    }
    else
    {
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

RAWINPUT* Embedder::getRawInput(HRAWINPUT lParam) {
    UINT dwSize = sizeof(RAWINPUT);
    static BYTE lpb[sizeof(RAWINPUT)];
    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
    return (RAWINPUT*)lpb;
}

void Embedder::postMsg(UINT msg, WPARAM wParam, LPARAM lParam) {
    auto win = MainWin::Get();
    PostMessage(win->hwnd, msg, wParam, lParam);
}
void Embedder::postMsg(UINT msg, WPARAM wParam, const POINT& point) {
    auto win = MainWin::Get();
    auto lParam = MAKELPARAM(point.x, point.y);
    PostMessage(win->hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Embedder::processMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg != WM_INPUT) {
        return CallWindowProc(oldProc, hWnd, uMsg, wParam, lParam);
    }
    POINT point;
    if (!checkMouseMsg(point)) return 0;
    auto raw = getRawInput((HRAWINPUT)lParam);
    if (raw->header.dwType == RIM_TYPEMOUSE)
    {
        RAWMOUSE rawMouse = raw->data.mouse;
        if (rawMouse.usButtonFlags == RI_MOUSE_WHEEL)
        {
            auto wheelDelta = (short)rawMouse.usButtonData;
            postMsg(WM_MOUSEWHEEL, wheelDelta, 0);
            return 0;
        }
        switch (rawMouse.ulButtons)
        {
            case RI_MOUSE_LEFT_BUTTON_DOWN:
            {
                postMsg(WM_LBUTTONDOWN, MK_LBUTTON, point);
                return 0;
            }
            case RI_MOUSE_LEFT_BUTTON_UP:
            {
                postMsg(WM_LBUTTONUP, MK_LBUTTON, point);
                return 0;
            }
            default:
            {
                postMsg(WM_MOUSEMOVE, 0x0020, point);
                return 0;
            }
        }
    }
    return CallWindowProc(oldProc, hWnd, uMsg, wParam, lParam);
}




void Embedder::roteInput()
{
    auto win = MainWin::Get();
    RAWINPUTDEVICE rids[1];
    rids[0].usUsagePage = 0x01;
    rids[0].usUsage = 0x02;
    rids[0].dwFlags = 0x00000100;
    rids[0].hwndTarget = win->hwnd;
    RegisterRawInputDevices(rids, 1, sizeof(rids[0]));
    oldProc = (WNDPROC)SetWindowLongPtr(win->hwnd, GWLP_WNDPROC, (LONG_PTR)Embedder::processMsg);
}
