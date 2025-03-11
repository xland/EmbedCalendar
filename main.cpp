#include <QtWidgets/QApplication>
#include <QTextCodec>
#include "MainWindow.h"
#include "Util.h"
#include "WsConn.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication a(argc, argv);
    Util::initDebuger();
    WsConn conn;
    auto win = new MainWindow(false);
    win->show();
    return a.exec();
}
