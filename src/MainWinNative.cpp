#include <string>
#include <format>
#include <windowsx.h>
#include "MainWin.h"
#include "App.h"
#include "Ctrl/TitleBar.h"

bool MainWin::EnableAlpha(HWND hwnd)
{
    if (!IsWindowsVistaOrGreater()) { return false; }
    BOOL is_composition_enable = false;
    DwmIsCompositionEnabled(&is_composition_enable);
    if (!is_composition_enable) { return true; }
    DWORD current_color = 0;
    BOOL is_opaque = false;
    DwmGetColorizationColor(&current_color, &is_opaque);
    if (!is_opaque || IsWindows8OrGreater())
    {
        HRGN region = CreateRectRgn(0, 0, -1, -1);
        DWM_BLURBEHIND bb = { 0 };
        bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
        bb.hRgnBlur = region;
        bb.fEnable = TRUE;
        DwmEnableBlurBehindWindow(hwnd, &bb);
        DeleteObject(region);
        return true;
    }
    else // For Window7
    {
        DWM_BLURBEHIND bb = { 0 };
        bb.dwFlags = DWM_BB_ENABLE;
        DwmEnableBlurBehindWindow(hwnd, &bb);
        return false;
    }
}

void MainWin::createWindow()
{
    static int num = 0;
    std::wstring clsName = std::format(L"EmbedCalendar{}", num++);
    auto hinstance = App::Get()->instance;
    WNDCLASSEX wcx{};
    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcx.lpfnWndProc = &MainWin::routeWinMsg;
    wcx.cbWndExtra = sizeof(MainWin*);
    wcx.hInstance = hinstance;
    wcx.hIcon = LoadIcon(hinstance, IDI_APPLICATION);
    wcx.hCursor = LoadCursor(hinstance, IDC_CROSS);
    wcx.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wcx.lpszClassName = clsName.c_str();
    RegisterClassEx(&wcx);
    hwnd = CreateWindowEx(NULL, clsName.c_str(), clsName.c_str(), WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP,
        x, y, w, h, NULL, NULL, hinstance, static_cast<LPVOID>(this));
    EnableAlpha(hwnd);
    App::Cursor(IDC_ARROW);
    ShowWindow(hwnd, SW_SHOW);
}

LRESULT MainWin::routeWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* pCS = reinterpret_cast<CREATESTRUCT*>(lParam);
        LPVOID pThis = pCS->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else if (msg == WM_SETCURSOR) {
        return true;
    }
    //else if (msg == WM_ERASEBKGND) {
    //    return true;
    //}
    //else if (msg == WM_PAINT) {
    //    return true;
    //}
    auto obj = reinterpret_cast<MainWin*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return obj->processNativeMsg(hWnd, msg, wParam, lParam);
}

LRESULT MainWin::processNativeMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg)
    {
        case WM_MOVE: {
            x = GET_X_LPARAM(lParam);
            y = GET_Y_LPARAM(lParam);
            break;
        }
        case WM_NCHITTEST: {
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            ScreenToClient(hwnd, &pt);
            auto flag = titleBar->IsInCaption(pt.x, pt.y);
            return flag ? HTCAPTION : HTCLIENT;
        }
        case WM_PAINT: {
            repaint();
            break;
        }
        case WM_LBUTTONDOWN:
        {
            isMouseDown = true;
            onLeftBtnDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        }
        case WM_LBUTTONUP:
        {
            isMouseDown = false;
            onLeftBtnUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        }
        case WM_MOUSEMOVE:
        {
            if (isMouseDown) {
                onMouseDrag(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }
            else
            {
                onMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }
            break;
        }
        case WM_DPICHANGED: {
            UINT dpiVal = HIWORD(wParam);
            dpi = dpiVal / 96.0f;
            RECT* const rect = (RECT*)lParam;
            x = rect->left;
            y = rect->top;
            w = 580*dpi;
            h = 580*dpi;
            initCanvas();
            SetWindowPos(hwnd, NULL,x,y,w, h,SWP_NOZORDER | SWP_NOACTIVATE);
            onDpiChange();
            Refresh();
            break;
        }
        case CustomMsgId: {
            onCustomMsg((EventType)wParam, (uint32_t)lParam);
            break;
        }
        case WM_TIMER: {
            if (wParam == RefreshTimerId) {
                KillTimer(hwnd, RefreshTimerId);
                refreshFlag = false;
                InvalidateRect(hwnd, nullptr, false);
            }
            break;
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}