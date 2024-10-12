#include <string>
#include <format>
#include <windowsx.h>
#include "MainWin.h"
#include "Util.h"
#include "WsConn.h"
#include "Ctrl/SwitchBtn.h"
#include "Ctrl/TitleBar.h"

void MainWin::createWindow()
{
    static int num = 0;
    std::wstring clsName = std::format(L"EmbedCalendar{}", num++);
    WNDCLASSEX wcx{};
    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc = &MainWin::routeWinMsg;
    wcx.cbWndExtra = sizeof(MainWin*);
    wcx.hInstance = instance;
    wcx.hIcon = LoadIcon(instance, IDI_APPLICATION);
    wcx.hCursor = LoadCursor(instance, IDC_CROSS); 
    wcx.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wcx.lpszClassName = clsName.c_str();
    RegisterClassEx(&wcx);
    hwnd = CreateWindowEx(WS_EX_TOOLWINDOW, clsName.c_str(), L"HikLink桌面日历", WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP,
        x, y, w, h, NULL, NULL, instance, static_cast<LPVOID>(this));
    Util::EnableAlpha(hwnd);
    MainWin::Cursor(IDC_ARROW);


    //HWND hWndDesktop = GetDesktopWindow();
    static HWND hwndDefView;
        EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        hwndDefView = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
        if (hwndDefView != NULL) {
            return FALSE;
        }
        return TRUE;
        },NULL);
    SetWindowLongPtr(hwnd, GWLP_HWNDPARENT, (LONG_PTR)hwndDefView);
    SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
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
    auto obj = reinterpret_cast<MainWin*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return obj->processNativeMsg(hWnd, msg, wParam, lParam);
}

LRESULT MainWin::processNativeMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg)
    {
        case WM_CLOSE: {
            Close();
            return 1;
        }
        case WM_WINDOWPOSCHANGING:
        {
            WINDOWPOS* pPos = reinterpret_cast<WINDOWPOS*>(lParam);
            // 确保不改变 Z 序
            pPos->flags |= SWP_NOZORDER;
            break;
        }
        case WM_SETFOCUS:{
            SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
            return 0;
        }
        case WM_ACTIVATE:
        {
            SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); //| SWP_NOACTIVATE
            return 0;
        }
        case WM_SIZE:{
            if (wParam == SIZE_MINIMIZED) {
                MessageBox(hwnd, L"Window Minimized", L"Notification", MB_OK);
            }
            return 0;
        }
        case WM_MOVE: {
            x = GET_X_LPARAM(lParam);
            y = GET_Y_LPARAM(lParam);
            break;
        }
        case WM_NCHITTEST: {
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            ScreenToClient(hWnd, &pt);
            auto flag = TitleBar::Get()->IsInCaption(pt.x, pt.y);
            return flag ? HTCAPTION : HTCLIENT;
        }
        case WM_PAINT: {
            repaint();
            break;
        }
        case WM_LBUTTONDOWN:
        {
            SetCapture(hWnd);
            onLeftBtnDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        }
        case WM_LBUTTONUP:
        {
            onLeftBtnUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            ReleaseCapture();
            break;
        }
        case WM_MOUSEMOVE:
        {
            if (!isTrackMouseEvent) {
                TRACKMOUSEEVENT tme = {};
                tme.cbSize = sizeof(TRACKMOUSEEVENT);
                tme.dwFlags = TME_HOVER|TME_LEAVE;
                tme.hwndTrack = hWnd;
                tme.dwHoverTime = HOVER_DEFAULT;
                isTrackMouseEvent = TrackMouseEvent(&tme);
            }
            onMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        }
        case WM_MOUSEWHEEL: {
            auto span = GET_WHEEL_DELTA_WPARAM(wParam);
            onMouseWheel(span);
            break;
        }
        case WM_MOUSELEAVE: {
            TRACKMOUSEEVENT tme = {};
            tme.cbSize = sizeof(TRACKMOUSEEVENT);
            tme.dwFlags = TME_CANCEL | TME_HOVER | TME_LEAVE;
            tme.hwndTrack = hWnd;
            TrackMouseEvent(&tme);
            isTrackMouseEvent = false;
            onMouseMove(-888888, -888888);
            break;
        }
        case WM_DPICHANGED: {
            UINT dpiVal = HIWORD(wParam);
            dpi = dpiVal / 96.0f;
            RECT* const rect = (RECT*)lParam;
            x = rect->left;
            y = rect->top;
            w = 580*dpi;
            if (SwitchBtn::Get()->listVisible) {
                h = 860 * dpi;
            }
            else {
                h = 580 * dpi;  //todo 
            }            
            initCanvas();
            SetWindowPos(hWnd, NULL,x,y,w, h,SWP_NOZORDER | SWP_NOACTIVATE);
            onDpiChange();
            Refresh();
            break;
        }
        case WM_TIMER: {
            if (wParam == RefreshTimerId) {
                KillTimer(hWnd, RefreshTimerId);
                refreshFlag = false;
                InvalidateRect(hWnd, nullptr, false);
            }
            else if (wParam == RefreshDataTimerId) {
                WsConn::Get()->PostMsg(R"({"msgName":"updateRenderData"})");
            }
            break;
        }
        case DataReadyId: {
            auto d = (rapidjson::Document*)lParam;
            onDataReady(d);
            break;
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}