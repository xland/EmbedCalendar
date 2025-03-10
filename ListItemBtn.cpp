#include <QPaintEvent>
#include <QPainter>

#include "MainWindow.h"
#include "Skin.h"
#include "Util.h"
#include "TipInfo.h"
#include "ListItemBtn.h"

ListItemBtn::ListItemBtn(const uint& code, QWidget* parent) : QWidget(parent), code{code}
{
    setFixedSize(26, 26);
    setMouseTracking(true);
    setCursor(Qt::CursorShape::PointingHandCursor);
}

ListItemBtn::~ListItemBtn()
{
	
}

void ListItemBtn::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto skin = Skin::get();
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    auto font = Util::getIconFont(14);
    painter.setFont(*font);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(skin->listItemBtn);
    painter.drawText(rect(), Qt::AlignCenter, QChar(code));
}


void ListItemBtn::enterEvent(QEvent* event)
{
    if (!isEnter) {
        emit enter();
        isEnter = true;
    }    
}

void ListItemBtn::leaveEvent(QEvent* event)
{
    if (isEnter) {
        auto win = (MainWindow*)window();
        win->tipInfo->hide();
        isEnter = false;
    }
}

void ListItemBtn::mousePressEvent(QMouseEvent* event)
{
}

void ListItemBtn::mouseReleaseEvent(QMouseEvent* event)
{
}
