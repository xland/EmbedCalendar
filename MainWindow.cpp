#include <QFont>
#include <QFontDatabase>
#include <Windows.h>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QPainter>

#include "Skin.h"
#include "MainWindow.h"
#include "TitleBar.h"
#include "YearBar.h"
#include "WeekBar.h"
#include "DayBtn.h"
#include "SwitchBar.h"
#include "ListBar.h"
#include "ListContent.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setFixedSize(QSize(372, 730));
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
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

    //auto workerW = getWorkerW();
    //auto hwnd = (HWND)winId();
    //SetParent(hwnd, workerW);
}
MainWindow::~MainWindow()
{
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor(255, 255, 255, 153));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 4, 4);
    QMainWindow::paintEvent(event);    
}

void MainWindow::embed()
{
}

HWND MainWindow::getWorkerW()
{
    static HWND workerW;
    if (!workerW) {
        HWND progman = FindWindow(L"Progman", NULL);
        SendMessage(progman, 0x052C, 0xD, 0);
        SendMessage(progman, 0x052C, 0xD, 1);
        EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
            HWND defView = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
            if (defView != NULL) {
                auto tar = (HWND*)lParam;
                *tar = FindWindowEx(NULL, hwnd, L"WorkerW", NULL);
            }
            return TRUE;
            }, (LPARAM)&workerW);
    }
    return workerW;
}
