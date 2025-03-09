#include <QPaintEvent>
#include <QPainter>

#include "Skin.h"
#include "Util.h"
#include "ListBarBtn.h"

ListBarBtn::ListBarBtn(const uint& code, QWidget* parent) : QWidget(parent)
{
    setFixedSize(28, 28);
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
    if (isHover) {
        painter.setBrush(skin->titleBtnHover);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(rect(), 2, 2);
    }
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    auto font = Util::getIconFont();
    font->setPixelSize(18);
    painter.setFont(*font);
    painter.setPen(skin->titleBtn);
    painter.drawText(rect(), Qt::AlignCenter, QChar(code));
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
