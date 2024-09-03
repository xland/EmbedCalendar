#include <string>
#include <format>
#include <windowsx.h>

#include "MainWin.h"
#include "App.h"


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
    wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcx.lpszClassName = clsName.c_str();
    RegisterClassEx(&wcx);
    hwnd = CreateWindowEx(WS_EX_LAYERED, clsName.c_str(), clsName.c_str(),
        WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP,
        x, y, w, h, NULL, NULL, hinstance, static_cast<LPVOID>(this));
    ShowWindow(hwnd, SW_SHOW);
    App::Cursor(IDC_ARROW);
    repaint();
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
    if (obj)
    {
        obj->processNativeMsg(msg,wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void MainWin::processNativeMsg(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg)
    {
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
            w = rect->right - rect->left;
            h = rect->bottom - rect->top;
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
                repaint();
            }
            break;
        }
    }
}