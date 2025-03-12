#include <QPainter>
#include <QPainterPath>

#include "WsConn.h"
#include "Util.h"
#include "Skin.h"
#include "TipInfo.h"
#include "MainWindow.h"
#include "ListItemBtn.h"
#include "ListItem.h"

ListItem::ListItem(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);


	setFixedSize(parent->width(),44);
    auto w = parent->width();
    delBtn = new ListItemBtn(0xe712, this);
    delBtn->move(w-34, 10);
    editBtn = new ListItemBtn(0xe707, this);
    editBtn->move(w-62,10);

    connect(delBtn, &ListItemBtn::enter, this, &ListItem::enterDel);
    connect(editBtn, &ListItemBtn::enter, this, &ListItem::enterEdit);
    connect(delBtn, &ListItemBtn::leave, this, &ListItem::leaveBtn);
    connect(editBtn, &ListItemBtn::leave, this, &ListItem::leaveBtn);
}

ListItem::~ListItem()
{
}

void ListItem::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    qreal radius = 3.0;

    auto skin = Skin::get();
    if (isHover) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(skin->listItemBg);
        painter.drawRoundedRect(rect(), radius, radius);
    }
    {
        QRectF rect(0, 0, 3, 44);        
        QPainterPath path;
        path.moveTo(rect.left() + radius, rect.top()); // 移动到左上角的圆弧起点
        path.arcTo(rect.left(), rect.top(), radius * 2, radius * 2, 90, 90); // 左上圆角
        path.lineTo(rect.left(), rect.bottom() - radius); // 左侧直线
        path.arcTo(rect.left(), rect.bottom() - radius * 2, radius * 2, radius * 2, 180, 90); // 左下圆角
        path.lineTo(rect.right(), rect.bottom()); // 底部直线
        path.lineTo(rect.right(), rect.top()); // 右侧直线
        path.closeSubpath();
        painter.setPen(Qt::NoPen);
        painter.setBrush(calendarColor);
        painter.drawPath(path);
    }

    painter.setRenderHint(QPainter::TextAntialiasing, true);
    auto font = Util::getTextFont(14);
    painter.setFont(*font);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(skin->listItemText1);
    painter.drawText(QPoint(8, 16), title);

    font->setPixelSize(12);
    painter.setFont(*font);
    painter.setPen(skin->listItemText2);
    painter.drawText(QPoint(8, 38), desc);
}

void ListItem::enterEvent(QEvent* event)
{
    if (!isHover) {
        isHover = true;
        update();
    }
}

void ListItem::leaveEvent(QEvent* event)
{
    if (isHover) {
        isHover = false;
        update();
    }
}

void ListItem::enterEdit()
{
    auto p = (QWidget*)(parent()->parent()->parent());
    auto rect = p->geometry();
    auto btnPos = editBtn->mapTo(window(), QPoint(0, 0));
    if (!rect.contains(btnPos)) return;
    auto win = (MainWindow*)window();
    auto str = WsConn::get()->data["lang"].toObject()["editSchedule"].toString();
    win->tipInfo->setText(str);
    auto pos = mapTo(win, editBtn->pos());
    pos.setX(pos.x() - win->tipInfo->width());
    pos.setY(pos.y() - 4);
    win->tipInfo->showInfo(pos);
}

void ListItem::enterDel()
{
    auto p = (QWidget*)(parent()->parent()->parent());
    auto rect = p->geometry();
    auto btnPos = editBtn->mapTo(window(), QPoint(0, 0));
    if (!rect.contains(btnPos)) return;
    auto win = (MainWindow*)window();
    auto str = WsConn::get()->data["lang"].toObject()["deleteSchedule"].toString();
    win->tipInfo->setText(str);
    auto pos = mapTo(win, delBtn->pos());
    pos.setX(pos.x() - win->tipInfo->width());
    pos.setY(pos.y() - 4);
    win->tipInfo->showInfo(pos);
}

void ListItem::leaveBtn()
{
    auto win = (MainWindow*)window();
    win->tipInfo->hide();
}
