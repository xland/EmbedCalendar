#include <QPaintEvent>
#include <QPainter>

#include "MainWindow.h"
#include "TipInfo.h"
#include "Skin.h"
#include "Util.h"
#include "ListBarBtn.h"

ListBarBtn::ListBarBtn(QWidget* parent) : QWidget(parent)
{
    setGeometry(parent->width()-26,0, 26, 26);
    setMouseTracking(true);
    setCursor(Qt::CursorShape::PointingHandCursor);
}

ListBarBtn::~ListBarBtn()
{
	
}

void ListBarBtn::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto skin = Skin::get();
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    auto font = Util::getIconFont(20);
    painter.setFont(*font);
    painter.setPen(skin->switchText);
    painter.drawText(rect(), Qt::AlignCenter, QChar(0xe70b));
}


void ListBarBtn::enterEvent(QEvent* event)
{
    if (!isHover) {
        isHover = true;
        auto win = (MainWindow*)window();
        win->tipInfo->showInfo(QString::fromLocal8Bit("新建日程"), QPoint(246,432));
    }
}

void ListBarBtn::leaveEvent(QEvent* event)
{
    if (isHover) {
        isHover = false;
        auto win = (MainWindow*)window();
        win->tipInfo->hide();
    }
}

void ListBarBtn::mousePressEvent(QMouseEvent* event)
{
}
