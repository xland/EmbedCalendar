#include <QPaintEvent>
#include <QPainter>

#include "Skin.h"
#include "Util.h"
#include "TitleBarBtn.h"

TitleBarBtn::TitleBarBtn(const uint& code, QWidget* parent) : QWidget(parent), code{code}
{
    setFixedSize(28, 28);
    setMouseTracking(true);
    setCursor(Qt::CursorShape::PointingHandCursor);
}

TitleBarBtn::~TitleBarBtn()
{
	
}

void TitleBarBtn::paintEvent(QPaintEvent* event)
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
    auto font = Util::getIconFont(18);
    painter.setFont(*font);
    painter.setPen(skin->titleBtn);
    painter.drawText(rect(), Qt::AlignCenter, QChar(code));
}


void TitleBarBtn::enterEvent(QEvent* event)
{
    if (!isHover) {
        isHover = true;
        update();
    }
}

void TitleBarBtn::leaveEvent(QEvent* event)
{
    if (isHover) {
        isHover = false;
        update();
    }
}

void TitleBarBtn::mousePressEvent(QMouseEvent* event)
{
}

void TitleBarBtn::mouseReleaseEvent(QMouseEvent* event)
{
}
