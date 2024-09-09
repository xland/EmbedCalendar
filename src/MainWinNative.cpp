#include <string>
#include <format>
#include <windowsx.h>
#include "MainWin.h"
#include "Util.h"
#include "Embedder.h"
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
    hwnd = CreateWindowEx(NULL, clsName.c_str(), L"HikLink桌面日历", WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP,
        x, y, w, h, NULL, NULL, instance, static_cast<LPVOID>(this));
    Util::EnableAlpha(hwnd);
    MainWin::Cursor(IDC_ARROW);
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
    else if (msg == WM_SETTINGCHANGE) {
        return true;
    }
    //else if (msg == WM_ERASEBKGND) {
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
            isMouseDown = true;
            SetCapture(hWnd);
            onLeftBtnDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        }
        case WM_LBUTTONUP:
        {
            isMouseDown = false;
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
            if (isMouseDown) {
                onMouseDrag(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }
            else
            {
                onMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }
            break;
        }
        case WM_MOUSELEAVE: {
            if (Embedder::Get()->isEmbedded) return 0;
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
        case CustomMsgId: {
            onCustomMsg(wParam, lParam);
            break;
        }
        case WM_TIMER: {
            if (wParam == RefreshTimerId) {
                KillTimer(hWnd, RefreshTimerId);
                refreshFlag = false;
                InvalidateRect(hWnd, nullptr, false);
            }
            else if (wParam == CheckWallPaperTimerId) {
                Embedder::Get()->TimerCB();
            }
            else if (wParam == RefreshDataTimerId) {
                std::cout << "1 hour later" << std::endl;
                WsConn::Get()->PostMsg(R"({"msgName":"updateRenderData"})");
            }
            break;
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}