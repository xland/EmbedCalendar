#include <QPainter>
#include <QPainterPath>

#include "WsConn.h"
#include "Util.h"
#include "Skin.h"
#include "TipInfo.h"
#include "MainWindow.h"
#include "ListItemBtn.h"
#include "ListItemCheckBtn.h"
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
    scheduleNo = data["scheduleNo"].toString();
    calendarNo = data["calendarNo"].toString();
    calendarColor.setNamedColor(data["calendarColor"].toString());
    isAllowEdit = data["isAllowEdit"].toBool();
	isAllowView = data["isAllowView"].toBool();
    if (isAllowEdit) {
        delBtn = new ListItemBtn(0xe712, this);
        delBtn->setVisible(false);
        delBtn->move(width() - 34, 10);
        editBtn = new ListItemBtn(0xe707, this);
        editBtn->setVisible(false);
        editBtn->move(width() - 62, 10);
        connect(delBtn, &ListItemBtn::enter, this, &ListItem::enterDel);
        connect(editBtn, &ListItemBtn::enter, this, &ListItem::enterEdit);
        connect(delBtn, &ListItemBtn::click, this, &ListItem::clickDel);
        connect(editBtn, &ListItemBtn::click, this, &ListItem::clickEdit);
        connect(delBtn, &ListItemBtn::leave, this, &ListItem::leaveBtn);
        connect(editBtn, &ListItemBtn::leave, this, &ListItem::leaveBtn);
    }
    auto str = data["platformCode"].toString();
    if (str == "HIKKEUSER" || str == "HIKKEPROJ") {
		checkBtn = new ListItemCheckBtn(this);
        checkBtn->taskProcess = data["taskProcess"].toInt();
		checkBtn->move(5, 1);
        connect(checkBtn, &ListItemBtn::enter, this, &ListItem::enterCheck);
        connect(checkBtn, &ListItemBtn::leave, this, &ListItem::leaveBtn);
        connect(checkBtn, &ListItemBtn::click, this, &ListItem::clickCheck);
	}
}

void ListItem::enterEvent(QEvent* event)
{
    BtnBase::enterEvent(event);
    if (delBtn) {
        delBtn->show();
        editBtn->show();
    }
}

void ListItem::leaveEvent(QEvent* event)
{
    BtnBase::leaveEvent(event);
    if (delBtn) {
        delBtn->hide();
        editBtn->hide();
    }
}

void ListItem::mousePressEvent(QMouseEvent* event)
{
	BtnBase::mousePressEvent(event);
	if (isAllowView) {
        QString msg{ R"({"msgType":"EmbedCalendar","msgName":"viewSchedule","data":{"scheduleNo":"%1","calendarNo":"%2"}})" };
        msg = msg.arg(scheduleNo).arg(calendarNo);
        WsConn::get()->sendMsg(msg);
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


    QFont font = *Util::getTextFont(12);
    painter.setFont(font);
    painter.setPen(skin->listItemText2);
    QFontMetrics metrics2(font);
    if (metrics2.horizontalAdvance(desc) > w) {
        desc = metrics2.elidedText(desc, Qt::ElideRight, w);
    }
    painter.drawText(QPoint(8, 38), desc);

    font.setPixelSize(14);
    if (checkBtn && checkBtn->taskProcess == 0) {
        font.setStrikeOut(true);
    }
    painter.setFont(font);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(skin->listItemText1);
    QFontMetrics metrics(font);
    if (metrics.horizontalAdvance(title) > w) {
        title = metrics.elidedText(title, Qt::ElideRight, w);
    }
    painter.drawText(QPoint(checkBtn ? 26 : 8, 16), title);
}

void ListItem::enterCheck()
{
    auto p = (QWidget*)(parent()->parent()->parent());
    auto rect = p->geometry();
    auto btnPos = checkBtn->mapTo(window(), QPoint(0, 0));
    if (!rect.contains(btnPos)) return;
    auto win = (MainWindow*)window();
    auto tipObj = TipInfo::get();
    tipObj->setText(checkBtn->taskProcess?clickToCompleteTodo: clickToRestartTodo);
    auto pos = mapTo(win, checkBtn->pos());
    pos.setX(pos.x());
    pos.setY(pos.y() - tipObj->height());
    tipObj->showInfo(pos);
}

void ListItem::clickCheck()
{
    QString msg{ R"({"msgType":"EmbedCalendar","msgName":"updatePersonTodo","data":{"scheduleNo":"%1","calendarNo":"%2","taskProcess":%3}})" };
    msg = msg.arg(scheduleNo).arg(calendarNo).arg(checkBtn->taskProcess);
    WsConn::get()->sendMsg(msg);
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


void ListItem::clickEdit()
{
    QString msg{ R"({"msgType":"EmbedCalendar","msgName":"updateSchedule","data":{"scheduleNo":"%1","calendarNo":"%2"}})" };
    msg = msg.arg(scheduleNo).arg(calendarNo);
    WsConn::get()->sendMsg(msg);
}

void ListItem::clickDel()
{
    QString msg{ R"({"msgType":"EmbedCalendar","msgName":"deleteSchedule","data":{"scheduleNo":"%1","calendarNo":"%2"}})" };
    msg = msg.arg(scheduleNo).arg(calendarNo);
    WsConn::get()->sendMsg(msg);
}

void ListItem::leaveBtn()
{
    TipInfo::get()->hide();
}
