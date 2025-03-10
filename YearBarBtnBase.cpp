#include <QPaintEvent>
#include <QPainter>

#include "MainWindow.h"
#include "TipInfo.h"
#include "Skin.h"
#include "Util.h"
#include "YearBarBtnBase.h"

YearBarBtnBase::YearBarBtnBase(QWidget* parent) : QWidget(parent)
{
    auto skin = Skin::get();
    hoverColor = skin->yearBtnHover;
    borderColor = skin->yearBtnBorder;
    color = skin->yearBtn;
	setFixedSize(22, 22);
    setMouseTracking(true);
    setCursor(Qt::CursorShape::PointingHandCursor);
}

YearBarBtnBase::~YearBarBtnBase()
{
	
}

void YearBarBtnBase::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto r = rect().adjusted(2, 2, -2, -2);
    if (isHover) {
        painter.setBrush(hoverColor);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(r);
    }
    painter.setBrush(Qt::NoBrush);
    painter.setPen(borderColor);
    painter.drawEllipse(r);
}

void YearBarBtnBase::enterEvent(QEvent* event)
{
    if (!isHover) {
        isHover = true;
        update();
        emit enter();
    }    
}

void YearBarBtnBase::leaveEvent(QEvent* event)
{
    if (isHover) {
        isHover = false;
        update();
        auto win = (MainWindow*)window();
        win->tipInfo->hide();
    }    
}

void YearBarBtnBase::mousePressEvent(QMouseEvent* event)
{
}

void YearBarBtnBase::mouseReleaseEvent(QMouseEvent* event)
{
}
