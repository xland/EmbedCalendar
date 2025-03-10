#include <QFont>
#include <QFontDatabase>
#include <Windows.h>
#include <windowsx.h>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QWindow>
#include <QEvent>
#include <QTimer>
#include <QApplication>

#include "Skin.h"
#include "MainWindow.h"
#include "TitleBar.h"
#include "YearBar.h"
#include "WeekBar.h"
#include "DayBtn.h"
#include "SwitchBar.h"
#include "ListBar.h"
#include "Util.h"
#include "TipInfo.h"
#include "ListContent.h"

namespace {
    WNDPROC oldProc;
}
namespace {
    HHOOK hMouseHook;
    HWND tarHwnd;
}


MainWindow::MainWindow(bool isEmbeded,QWidget *parent) : QMainWindow(parent), isEmbeded{ isEmbeded }
{
	setFixedSize(QSize(372, 730));
	setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_TranslucentBackground, true);

    Skin::init();
    titleBar = new TitleBar(this);
    yearBar = new YearBar(this);
    weekBar = new WeekBar(this);
    for (int i=0;i<42;i++)
    {
        dayBtns.append(new DayBtn(i,this));
    }
    listBar = new ListBar(this);
    listContent = new ListContent(this);
    switchBar = new SwitchBar(this);
    tipInfo = new TipInfo(this);
    if (isEmbeded) {
        embed();
    }
    //QTimer::singleShot(2000, [this]() {
    //    QPoint p(330, 16);
    //    QWidget* child = childAt(p);
    //    QPoint g = mapToGlobal(p);
    //    QMouseEvent e(QEvent::MouseMove, QPoint(8,27), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
    //    QApplication::sendEvent(child, &e);
    //});



}
MainWindow::~MainWindow()
{
}

LRESULT MainWindow::mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    QPoint pos = QCursor::pos();
    LPARAM lParamNew = MAKELPARAM(330, 16);
    if (nCode >= 0) {
        if (wParam == WM_LBUTTONDOWN) {
            PostMessage(tarHwnd, nCode, wParam, lParamNew);
        }
        else if (wParam == WM_LBUTTONUP) {
            PostMessage(tarHwnd, nCode, wParam, lParamNew);
        }
        else if (wParam == WM_MOUSEMOVE) {
            PostMessage(tarHwnd, nCode, wParam, lParamNew);
        }
    }
    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}


void MainWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto skin = Skin::get();
    painter.setBrush(skin->bg);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 4, 4);  

    //painter.drawRect(QRect(325, 10, 30, 30));
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    deleteLater();
}

void MainWindow::switchEmbed()
{
    auto pos = this->pos();
    close();
    auto win = new MainWindow(!isEmbeded);
    win->move(pos);
    win->show();
}


void MainWindow::embed()
{
    auto workerW = Util::getWorkerW();
    auto hwnd = (HWND)winId();

    tarHwnd = hwnd;
    SetParent(hwnd, workerW);
    //hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, GetModuleHandle(NULL), 0);
    RAWINPUTDEVICE rids[1];
    rids[0].usUsagePage = 0x01;
    rids[0].usUsage = 0x02;
    rids[0].dwFlags = 0x00000100;
    rids[0].hwndTarget = hwnd;
    RegisterRawInputDevices(rids, 1, sizeof(rids[0]));
    oldProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)MainWindow::processMsg);
}

LRESULT CALLBACK MainWindow::processMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg != WM_INPUT) {
        return CallWindowProc(oldProc, hWnd, uMsg, wParam, lParam);
    }

    POINT globalPos;
    GetCursorPos(&globalPos);
    RECT rect;
    GetWindowRect(hWnd, &rect);
    if (globalPos.x < rect.left || globalPos.y < rect.top ||
        globalPos.x > rect.right || globalPos.y > rect.bottom) {
        return CallWindowProc(oldProc, hWnd, uMsg, wParam, lParam);
    }

    HWND hwnd = WindowFromPoint(globalPos);
    WCHAR className[28];
    int len = GetClassName(hwnd, className, 28);
    if ((lstrcmp(TEXT("SysListView32"), className) != 0) &&
        (lstrcmp(TEXT("WorkerW"), className) != 0) &&
        (lstrcmp(TEXT("Progman"), className) != 0)) {
        return CallWindowProc(oldProc, hWnd, uMsg, wParam, lParam);
    }

    auto wins = QApplication::topLevelWidgets();
    auto win = (MainWindow*)QApplication::topLevelWidgets().first();
    if (!win) {
        return CallWindowProc(oldProc, hWnd, uMsg, wParam, lParam);
    }
    auto raw = getRawInput((HRAWINPUT)lParam);
    if (raw->header.dwType == RIM_TYPEMOUSE)
    {
        RAWMOUSE rawMouse = raw->data.mouse;
        if (rawMouse.usButtonFlags == RI_MOUSE_WHEEL)
        {
            auto wheelDelta = (short)rawMouse.usButtonData;
            
            return 0;
        }
        switch (rawMouse.ulButtons)
        {
        case RI_MOUSE_LEFT_BUTTON_DOWN:
        {

            break;
        }
        case RI_MOUSE_LEFT_BUTTON_UP:
        {

            break;
        }
        default:
        {
            auto pos = win->mapFromGlobal(QCursor::pos());
            QWidget* child = win->childAt(pos);
            if (!child) {
                break;
            }
            auto childPos = child->mapFromGlobal(QCursor::pos());  // 转换到子控件坐标
            QMouseEvent e(QEvent::MouseMove, childPos, Qt::NoButton, Qt::NoButton, Qt::NoModifier);
            QApplication::sendEvent(child, &e);            
            break;
        }
        }
    }
    return CallWindowProc(oldProc, hWnd, uMsg, wParam, lParam);
}

RAWINPUT* MainWindow::getRawInput(HRAWINPUT lParam) {
    UINT dwSize = sizeof(RAWINPUT);
    static BYTE lpb[sizeof(RAWINPUT)];
    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
    return (RAWINPUT*)lpb;
}