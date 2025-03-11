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
#include <QJsonArray>

#include "Skin.h"
#include "MainWindow.h"
#include "BtnBase.h"
#include "TitleBar.h"
#include "YearBar.h"
#include "WeekBar.h"
#include "DayBtn.h"
#include "SwitchBtn.h"
#include "ListBar.h"
#include "Util.h"
#include "TipInfo.h"
#include "WsConn.h"
#include "ListContent.h"

namespace {
    WNDPROC oldProc;
    MainWindow* win;
}


MainWindow::MainWindow(bool isEmbeded,QWidget *parent) : QMainWindow(parent), isEmbeded{ isEmbeded }
{
	setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_TranslucentBackground, true);
    titleBar = new TitleBar(this);
    yearBar = new YearBar(this);
    weekBar = new WeekBar(this);
    auto arr = WsConn::get()->data["viewData"].toArray();
    for (int i=0;i<arr.size();i++)
    {
        auto obj = arr[i].toObject();
        auto day = new DayBtn(i, obj,this);
        dayBtns.append(day);
    }
    listBar = new ListBar(this);
    listContent = new ListContent(this);
    auto flag = WsConn::get()->data["displayScheduleList"].toBool();
    if (flag) {
        setFixedSize(QSize(372, 730));
    }
    else {
        listBar->hide();
        listContent->hide();
        setFixedSize(QSize(372, 480));
    }
    switchBtn = new SwitchBtn(this);
    tipInfo = new TipInfo(this);
    if (isEmbeded) {
        embed();
    }
}
MainWindow::~MainWindow()
{
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
    if (isEmbeded) {
        auto hwnd = (HWND)winId();
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)oldProc);
        SetParent(hwnd, nullptr);
    }
    auto pos = this->pos();
    close();
    win = new MainWindow(!isEmbeded);
    win->move(pos);
    win->show();
}


void MainWindow::onEmbedMouseMove()
{
    auto pos = mapFromGlobal(QCursor::pos());
    auto children = findChildren<BtnBase*>();
    for (auto& child:children)
    {
        auto rect = QRect(child->mapTo(this, QPoint(0, 0)), child->size());;
        if (rect.contains(pos)) {
            child->enterEvent(nullptr);
        }
        else {
            child->leaveEvent(nullptr);
        }
    }
}

void MainWindow::onEmbedMousePress()
{
    auto pos = mapFromGlobal(QCursor::pos());
    auto child = dynamic_cast<BtnBase*>(childAt(pos));
    if (!child) return;
    QMouseEvent e(QEvent::MouseButtonPress, QPointF(0, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    child->mousePressEvent(&e);
}

void MainWindow::onEmbedMouseWheel(const int& wheelData)
{
    auto pos = mapFromGlobal(QCursor::pos());
    if (listContent->geometry().contains(pos)) {
        listContent->scroll(wheelData);
    }
}

void MainWindow::onEmbedLeaveWindow()
{
    if (!isEnter) return;
    auto children = findChildren<BtnBase*>();
    for (auto& child : children)
    {
        child->leaveEvent(nullptr);
    }
    isEnter = false;
}

void MainWindow::embed()
{
    auto workerW = Util::getWorkerW();
    auto hwnd = (HWND)winId();
    SetParent(hwnd, workerW);

    QTimer::singleShot(1000, [hwnd]() {
        RAWINPUTDEVICE rids[1];
        rids[0].usUsagePage = 0x01;
        rids[0].usUsage = 0x02;
        rids[0].dwFlags = 0x00000100;
        rids[0].hwndTarget = hwnd;
        RegisterRawInputDevices(rids, 1, sizeof(rids[0]));
        oldProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)MainWindow::processMsg);
    });

}

LRESULT CALLBACK MainWindow::processMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //auto win = (MainWindow*)QApplication::topLevelWidgets().first();  //  || !win 
    if (uMsg != WM_INPUT ) {
        return CallWindowProc(oldProc, hWnd, uMsg, wParam, lParam);
    }
    POINT globalPos;
    GetCursorPos(&globalPos);
    RECT rect;
    GetWindowRect(hWnd, &rect);
    if (globalPos.x < rect.left || globalPos.y < rect.top ||
        globalPos.x > rect.right || globalPos.y > rect.bottom) {
        win->onEmbedLeaveWindow();
        return CallWindowProc(oldProc, hWnd, uMsg, wParam, lParam);
    }

    HWND hwnd = WindowFromPoint(globalPos);
    WCHAR className[28];
    int len = GetClassName(hwnd, className, 28);
    if ((lstrcmp(TEXT("SysListView32"), className) != 0) &&
        (lstrcmp(TEXT("WorkerW"), className) != 0) &&
        (lstrcmp(TEXT("Progman"), className) != 0)) {
        win->onEmbedLeaveWindow();
        return CallWindowProc(oldProc, hWnd, uMsg, wParam, lParam);
    }
    win->isEnter = true;
    auto raw = getRawInput((HRAWINPUT)lParam);
    if (raw->header.dwType == RIM_TYPEMOUSE)
    {
        RAWMOUSE rawMouse = raw->data.mouse;
        if (rawMouse.usButtonFlags == RI_MOUSE_WHEEL)
        {
            auto wheelDelta = (short)rawMouse.usButtonData;
            win->onEmbedMouseWheel(wheelDelta);
        }
        else {
            switch (rawMouse.ulButtons)
            {
            case RI_MOUSE_LEFT_BUTTON_DOWN:
            {
                win->onEmbedMousePress();
                break;
            }
            default:
            {
                win->onEmbedMouseMove();
                break;
            }
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

void MainWindow::init()
{
    auto& data = WsConn::get()->data;
    auto flag = data["hasEmbed"].toBool();
    win = new MainWindow(flag);
    win->show();
}

MainWindow* MainWindow::get()
{
    return win;
}
