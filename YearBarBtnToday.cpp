#include <QPaintEvent>
#include <QPainter>

#include "Skin.h"
#include "Util.h"
#include "YearBarBtnToday.h"

YearBarBtnToday::YearBarBtnToday(QWidget* parent) : YearBarBtnBase(parent)
{
    auto skin = Skin::get();
    hoverColor = skin->yearBtnTodayHover;
    borderColor = skin->yearBtnTodayBorder;
    color = skin->yearBtnToday;
}

YearBarBtnToday::~YearBarBtnToday()
{
	
}

void YearBarBtnToday::paintEvent(QPaintEvent* event)
{
    YearBarBtnBase::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    auto r = rect().adjusted(2, 2, -2, -2);
    auto font = Util::getIconFont();
    font->setPixelSize(12);
    painter.setFont(*font);
    painter.setPen(color);
    painter.drawText(r, Qt::AlignCenter, QString::fromLocal8Bit("今"));
}