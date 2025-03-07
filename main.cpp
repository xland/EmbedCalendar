#include "EmbedCalendar.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EmbedCalendar w;
    w.show();
    return a.exec();
}
