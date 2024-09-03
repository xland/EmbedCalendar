#include "EmbedHelper.h"
#include <hidusage.h>
#include <cstdint>
#include <format>
#include <iostream>

namespace {
	HWND tarHwnd{ nullptr };
    bool isEmbeded{ false };
    WNDPROC OldProc;
    RECT tarRect;
    EmbedHelper* instance;
}



void LogMessage(const std::string&& message) {
    OutputDebugStringA(message.c_str());
    OutputDebugStringA("\r\n");
}

POINT getMousePosInWin() {
    POINT point;
    GetCursorPos(&point);
    if (point.x > tarRect.left && point.y > tarRect.top && point.x < tarRect.right && point.y < tarRect.bottom) {
        point.x = point.x - tarRect.left;
        point.y = point.y - tarRect.top;
    }
    else
    {
        point.x = -100;
        point.y = -100;
    }
    return point;
}

//void sendEvent(QObject* parent, QMouseEvent* event) {
//    for (QObject* child : parent->children()) {
//        QQuickItem* childItem = qobject_cast<QQuickItem*>(child);
//        if (childItem) {
//            QCoreApplication::postEvent(childItem, event);
//            sendEvent(childItem, event);
//        }
//    }
//}
bool isMouseOnDesktop() {
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

LRESULT CALLBACK handleWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_INPUT && isEmbeded)
    {
        POINT point = getMousePosInWin();
        if (point.x < 0 && point.y < 0 && isEmbeded) {
            return 0;
        }
        if (!isMouseOnDesktop()) {
            return 0;
        }
        //auto ratio = window->screen()->devicePixelRatio();
        UINT dwSize = sizeof(RAWINPUT);
        static BYTE lpb[sizeof(RAWINPUT)];
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
        auto* raw = (RAWINPUT*)lpb;
        switch (raw->header.dwType)
        {
            case RIM_TYPEMOUSE:
            {
                RAWMOUSE rawMouse = raw->data.mouse;
                if (rawMouse.usButtonFlags == RI_MOUSE_WHEEL)
                {
                    //point.x = point.x / ratio;
                    //point.y = point.y / ratio;
                    //auto wheelDelta = (float)(short)rawMouse.usButtonData; 
                    //QMetaObject::invokeMethod(window, "wheelFunc", Q_ARG(QVariant, wheelDelta > 0), Q_ARG(QVariant, (int)point.x), Q_ARG(QVariant, (int)point.y));
                    //break;
                }
                auto lParam = MAKELPARAM(point.x, point.y);
                switch (rawMouse.ulButtons)
                {
                    case RI_MOUSE_LEFT_BUTTON_DOWN:
                    {
                        //PostMessage(tarHwnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
                        //point.x = point.x / ratio;
                        //point.y = point.y / ratio;
                        //QMetaObject::invokeMethod(window, "downFunc", Q_ARG(QVariant, (int)point.x), Q_ARG(QVariant, (int)point.y));
                        //break;
                    }
                    case RI_MOUSE_LEFT_BUTTON_UP:
                    {
                        //PostMessage(tarHwnd, WM_LBUTTONUP, MK_LBUTTON, lParam);
                        //point.x = point.x / ratio;
                        //point.y = point.y / ratio;
                        //QMetaObject::invokeMethod(window, "upFunc", Q_ARG(QVariant, (int)point.x), Q_ARG(QVariant, (int)point.y));
                        break;
                    }
                    default:
                    {
                        //point.x = point.x / ratio;
                        //point.y = point.y / ratio;
                        //QMetaObject::invokeMethod(window, "moveFunc", Q_ARG(QVariant, (int)point.x), Q_ARG(QVariant, (int)point.y));
                        return 0;
                    }
                }
                break;
            }
        }
    }
    return CallWindowProc(OldProc, hWnd, uMsg, wParam, lParam);
}
void roteInput()
{
    RAWINPUTDEVICE rids[2];
    rids[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    rids[0].usUsage = HID_USAGE_GENERIC_MOUSE;
    rids[0].dwFlags = RIDEV_INPUTSINK;
    rids[0].hwndTarget = tarHwnd;

    rids[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
    rids[1].usUsage = HID_USAGE_GENERIC_KEYBOARD;
    rids[1].dwFlags = RIDEV_EXINPUTSINK;
    rids[1].hwndTarget = tarHwnd;
    RegisterRawInputDevices(rids, 2, sizeof(rids[0]));
    OldProc = (WNDPROC)SetWindowLongPtr(tarHwnd, GWLP_WNDPROC, (LONG_PTR)handleWindowMessage);
}

EmbedHelper::~EmbedHelper()
{
}

EmbedHelper* EmbedHelper::Init() {

    //window = static_cast<QQuickWindow*>(_root);
    //tarHwnd = (HWND)window->winId();
    //HWND hDesktop = GetDesktopWindow();
    //SetParent(tarHwnd, hDesktop);
    //SetWindowLong(tarHwnd, GWL_EXSTYLE, GetWindowLong(tarHwnd, GWL_EXSTYLE) | WS_EX_TOPMOST);
    //SetWindowPos(tarHwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    //instance = new EmbedHelper(_root);
    return instance;
}

void EmbedHelper::Embed() { 
    if (isEmbeded) {
        SetWindowLongPtr(tarHwnd, GWLP_WNDPROC, (LONG_PTR)OldProc);
        SetParent(tarHwnd, nullptr);
        isEmbeded = false;
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
        SetParent(tarHwnd, workerW);
        GetWindowRect(tarHwnd, &tarRect);

        //auto l = tarRect.left / 1.5;
        //auto t = tarRect.top / 1.5;
        roteInput();  //4040  1038
        isEmbeded = true;

        //DWORD workerWThreadId = GetWindowThreadProcessId(sysListView32HWND, nullptr);
        //DWORD targetThreadId = GetWindowThreadProcessId(tarHwnd, nullptr);
        //auto flag = AttachThreadInput(workerWThreadId, targetThreadId, TRUE);
        //SetForegroundWindow(tarHwnd);
        //SetFocus(tarHwnd);
    }
}
void EmbedHelper::WinResized()
{
    GetWindowRect(tarHwnd, &tarRect);
}

bool EmbedHelper::IsEmbed()
{
    return isEmbeded;
}
