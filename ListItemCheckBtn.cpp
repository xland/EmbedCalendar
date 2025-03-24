#include <QPainter>

#include "Util.h"
#include "Skin.h"
#include "ListItem.h"
#include "ListItemCheckBtn.h"

ListItemCheckBtn::ListItemCheckBtn(QWidget *parent) : BtnBase(parent)
{
	setFixedSize(20, 20);
}

ListItemCheckBtn::~ListItemCheckBtn()
{}

void ListItemCheckBtn::paintEvent(QPaintEvent * event)
{
    auto font = Util::getIconFont(14);
    QPainter painter(this);
    painter.setFont(*font);
    painter.setBrush(Qt::NoBrush);
    auto skin = Skin::get();
    if (taskProcess == -1) {
        painter.setPen(QColor(178, 178, 178));
        painter.drawText(rect(), Qt::AlignCenter, QChar(0xe72b));
    }
    else
    {
        auto item = (ListItem*)parent();
        painter.setPen(item->calendarColor);
        painter.drawText(rect(), Qt::AlignCenter, QChar(0xe72c));
    }
}
