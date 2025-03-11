
#include "BtnBase.h"

BtnBase::BtnBase(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    setCursor(Qt::CursorShape::PointingHandCursor);
}

BtnBase::~BtnBase()
{

}

void BtnBase::enterEvent(QEvent* event)
{
    if (!isHover) {
        isHover = true;
        update();
        emit enter();
    }
}

void BtnBase::leaveEvent(QEvent* event)
{
    if (isHover) {
        isHover = false;
        update();
        emit leave();
    }
}

void BtnBase::mousePressEvent(QMouseEvent* event)
{
    emit click();
}
