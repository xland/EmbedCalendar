#include <QFont>
#include <QFontDatabase>
#include <Windows.h>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

#include "Skin.h"
#include "MainWindow.h"
#include "TitleBar.h"
#include "YearBar.h"
#include "WeekBar.h"
#include "DayBtn.h"
#include "SwitchBar.h"
#include "ListBar.h"
#include "Util.h"
#include "ListContent.h"

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
    if (isEmbeded) {
        auto workerW = Util::getWorkerW();
        auto hwnd = (HWND)winId();
        SetParent(hwnd, workerW);
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
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    deleteLater();
}

void MainWindow::switchEmbed()
{
    close();
    auto win = new MainWindow(!isEmbeded);
    win->show();
}
