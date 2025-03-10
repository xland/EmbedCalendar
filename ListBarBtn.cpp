#include <QPaintEvent>
#include <QPainter>

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
        update();
    }
}

void ListBarBtn::leaveEvent(QEvent* event)
{
    if (isHover) {
        isHover = false;
        update();
    }
}

void ListBarBtn::mousePressEvent(QMouseEvent* event)
{
}

void ListBarBtn::mouseReleaseEvent(QMouseEvent* event)
{
}
