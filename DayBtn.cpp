#include <QPaintEvent>
#include <QPainter>

#include "Skin.h"
#include "Util.h"
#include "DayBtn.h"

DayBtn::DayBtn(const int& index, QWidget* parent) : QWidget(parent), index{index}
{
    int lineNum = index / 7;
    int colNum = index % 7;
    setGeometry(colNum * 44+14+6*colNum, lineNum * 44+136 + 4 * lineNum, 44, 44);
    setMouseTracking(true);
    setCursor(Qt::CursorShape::PointingHandCursor);
}

DayBtn::~DayBtn()
{
	
}

void DayBtn::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    auto skin = Skin::get();
    if (isHover) {
        painter.setBrush(skin->dayHover);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(rect());
    }
    auto font = Util::getTextFont(16);
    painter.setFont(*font);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(skin->day);

    QRect textRect = rect();
    textRect.setTop(textRect.top() + 4);
    QTextOption option;
    option.setAlignment(Qt::AlignHCenter);
    painter.drawText(textRect, QString::number(index), option);

    font->setPixelSize(10);
    painter.setFont(*font);
    textRect.setTop(textRect.top() + 19);
    painter.setPen(skin->lunar);
    painter.drawText(textRect, QString::fromLocal8Bit("惊蛰"), option);

    QRect r1 = rect();
    auto x = r1.right() - 11;
    auto y = r1.top() + 14;
    painter.setPen(skin->dayWorking);
    painter.drawText(QPoint(x, y), QString::fromLocal8Bit("休"));

    QRect r2(textRect.width() / 2 - 2, textRect.bottom() - 4, 4, 4);
    painter.setBrush(skin->dot);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(r2);
}


void DayBtn::enterEvent(QEvent* event)
{
    if (!isHover) {
        isHover = true;
        update();
    }
}

void DayBtn::leaveEvent(QEvent* event)
{
    if (isHover) {
        isHover = false;
        update();
    }
}

void DayBtn::mousePressEvent(QMouseEvent* event)
{
}

void DayBtn::mouseReleaseEvent(QMouseEvent* event)
{
}
