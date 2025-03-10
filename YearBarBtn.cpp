#include <QPaintEvent>
#include <QPainter>

#include "Util.h"
#include "YearBarBtn.h"

YearBarBtn::YearBarBtn(const uint& code, QWidget* parent) : YearBarBtnBase(parent), code{code}
{
}

YearBarBtn::~YearBarBtn()
{
	
}

void YearBarBtn::paintEvent(QPaintEvent* event)
{
    YearBarBtnBase::paintEvent(event);
    QPainter painter(this);
    auto r = rect().adjusted(2, 2, -2, -2);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    auto font = Util::getIconFont(12);
    painter.setFont(*font);
    painter.setPen(color);
    painter.drawText(r, Qt::AlignCenter, QChar(code));
}
