#include <QPainter>
#include <QPainterPath>

#include "WsConn.h"
#include "Util.h"
#include "Skin.h"
#include "TipInfo.h"
#include "MainWindow.h"
#include "ListItemBtn.h"
#include "ListItem.h"

ListItem::ListItem(QWidget *parent) : BtnBase(parent)
{
    setMouseTracking(true);
	setFixedSize(parent->width(),44);
}

ListItem::~ListItem()
{
}

void ListItem::setData(const QJsonObject& data)
{
    title = data["title"].toString();
    desc = data["desc"].toString();
    calendarColor.setNamedColor(data["calendarColor"].toString());
    isAllowEdit = data["isAllowEdit"].toBool();
    if (isAllowEdit) {
        delBtn = new ListItemBtn(0xe712, this);
        delBtn->move(width() - 34, 10);
        editBtn = new ListItemBtn(0xe707, this);
        editBtn->move(width() - 62, 10);
        connect(delBtn, &ListItemBtn::enter, this, &ListItem::enterDel);
        connect(editBtn, &ListItemBtn::enter, this, &ListItem::enterEdit);
        connect(delBtn, &ListItemBtn::leave, this, &ListItem::leaveBtn);
        connect(editBtn, &ListItemBtn::leave, this, &ListItem::leaveBtn);
    }
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

    auto w = width() - (isAllowEdit?80:16);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    auto font = Util::getTextFont(14);
    painter.setFont(*font);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(skin->listItemText1);
    QFontMetrics metrics(*font);
    if (metrics.horizontalAdvance(title) > w) {
        title = metrics.elidedText(title, Qt::ElideRight, w);
    }
    painter.drawText(QPoint(8, 16), title);

    font->setPixelSize(12);
    painter.setFont(*font);
    painter.setPen(skin->listItemText2);
    QFontMetrics metrics2(*font);
    if (metrics2.horizontalAdvance(desc) > w) {
        desc = metrics2.elidedText(desc, Qt::ElideRight, w);
    }
    painter.drawText(QPoint(8, 38), desc);
}

void ListItem::enterEdit()
{
    auto p = (QWidget*)(parent()->parent()->parent());
    auto rect = p->geometry();
    auto btnPos = editBtn->mapTo(window(), QPoint(0, 0));
    if (!rect.contains(btnPos)) return;
    auto win = (MainWindow*)window();
    auto tipObj = TipInfo::get();
    tipObj->setText(editTip);
    auto pos = mapTo(win, editBtn->pos());
    pos.setX(pos.x() - tipObj->width());
    pos.setY(pos.y() - 4);
    tipObj->showInfo(pos);
}

void ListItem::enterDel()
{
    auto p = (QWidget*)(parent()->parent()->parent());
    auto rect = p->geometry();
    auto btnPos = editBtn->mapTo(window(), QPoint(0, 0));
    if (!rect.contains(btnPos)) return;
    auto win = (MainWindow*)window();
    auto tipObj = TipInfo::get();
    tipObj->setText(delTip);
    auto pos = mapTo(win, delBtn->pos());
    pos.setX(pos.x() - tipObj->width());
    pos.setY(pos.y() - 4);
    tipObj->showInfo(pos);
}

void ListItem::leaveBtn()
{
    TipInfo::get()->hide();
}
