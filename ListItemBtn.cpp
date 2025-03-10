#include <QPaintEvent>
#include <QPainter>

#include "Skin.h"
#include "Util.h"
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
    if (isHover) {
        painter.setBrush(skin->titleBtnHover);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(rect(), 2, 2);
    }
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    auto font = Util::getIconFont(14);
    painter.setFont(*font);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(skin->listItemBtn);
    painter.drawText(rect(), Qt::AlignCenter, QChar(code));
}


void ListItemBtn::enterEvent(QEvent* event)
{
    if (!isHover) {
        isHover = true;
        update();
    }
}

void ListItemBtn::leaveEvent(QEvent* event)
{
    if (isHover) {
        isHover = false;
        update();
    }
}

void ListItemBtn::mousePressEvent(QMouseEvent* event)
{
}

void ListItemBtn::mouseReleaseEvent(QMouseEvent* event)
{
}
