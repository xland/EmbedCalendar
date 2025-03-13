#include <QtWidgets/QApplication>
#include <QTextCodec>
#include "MainWindow.h"
#include "Util.h"
#include "Skin.h"
#include "WsConn.h"
#include "TitleBar.h"
#include "YearBar.h"
#include "WeekBar.h"
#include "DayBtn.h"
#include "SwitchBtn.h"
#include "ListContent.h"
#include "ListBar.h"
#include "TipInfo.h"
#include "Menu.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication a(argc, argv);
    Util::initDebuger();
    auto conn = WsConn::init();
    Skin::init();
    MainWindow::init();
    TitleBar::init();
    Menu::init();
    YearBar::init();
    WeekBar::init();
    ListBar::init();
    ListContent::init();
    SwitchBtn::init();
    TipInfo::init();
    conn->startConnect();
    return a.exec();
}
