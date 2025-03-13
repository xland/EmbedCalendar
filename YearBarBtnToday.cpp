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
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    if (isCn) {
        auto r = rect().adjusted(28, 2, -2, -2);
        if (isHover) {
            painter.setBrush(hoverColor);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(r);
        }
        painter.setBrush(Qt::NoBrush);
        painter.setPen(borderColor);
        painter.drawEllipse(r);

        painter.setPen(color);
        painter.setBrush(Qt::NoBrush);
        auto font = Util::getTextFont(12);
        painter.setFont(*font);
        painter.drawText(r, Qt::AlignCenter, QString::fromLocal8Bit("今"));
    }
    else {
        auto r = rect().adjusted(1, 2, -1, -2);
        if (isHover) {
            painter.setBrush(hoverColor);
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(r, 10, 10);
        }
        painter.setBrush(Qt::NoBrush);
        painter.setPen(borderColor);
        painter.drawRoundedRect(r, 10, 10);

        painter.setPen(color);
        painter.setBrush(Qt::NoBrush);
        auto font = Util::getTextFont(12);
        painter.setFont(*font);
        painter.drawText(r, Qt::AlignCenter, "Today");
    }

}