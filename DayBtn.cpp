#include <QPaintEvent>
#include <QPainter>
#include <QJsonArray>

#include "WsConn.h"
#include "Skin.h"
#include "Util.h"
#include "DayBtn.h"

DayBtn::DayBtn(const int& index,QWidget* parent) : BtnBase(parent), index{index}
{

    int lineNum = index / 7;
    int colNum = index % 7;
    setGeometry(colNum * 44+14+6*colNum, lineNum * 44+136 + 4 * lineNum, 44, 44);
    setMouseTracking(true);
    setCursor(Qt::CursorShape::PointingHandCursor);
    connect(this, &DayBtn::click, this, &DayBtn::onClick);
}

DayBtn::~DayBtn()
{
	
}

void DayBtn::updateData(const QJsonObject& obj)
{
    year = obj["year"].toInt();
    month = obj["month"].toInt();
    date = obj["date"].toInt();
    lunar = obj["lunarInfo"].toString();
    docStatus = obj["docStatus"].toString();
    hasSchdule = obj["hasSchdule"].toBool();
    isActive = obj["isActive"].toBool();
    isToday = obj["isToday"].toBool();
    isCurMonth = obj["type"].toString() == "currt";
}

void DayBtn::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    auto skin = Skin::get();
    if (isActive) {
        painter.setBrush(skin->dayActive);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(rect());
    }
    else if (isToday) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(skin->dayActive);
        painter.drawEllipse(rect().adjusted(1,1,-1,-1));
    }
    if (!isActive && isHover) {
        painter.setBrush(skin->dayHover);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(rect());
    }
    auto font = Util::getTextFont(16);
    painter.setFont(*font);
    painter.setBrush(Qt::NoBrush);
    if (isActive) {
        painter.setPen(QColor(255, 255, 255));
    }
    else if (isToday) {
        painter.setPen(skin->dayActive);
    }
    else {
        painter.setPen(isCurMonth ? skin->day : skin->dayNotCurMonth);
    }
    QRect textRect = rect();
    textRect.setTop(textRect.top() + 4);
    QTextOption option;
    option.setAlignment(Qt::AlignHCenter);
    painter.drawText(textRect, QString::number(date), option);

    font->setPixelSize(10);
    painter.setFont(*font);
    textRect.setTop(textRect.top() + 19);
    if (isActive) {
        painter.setPen(QColor(255,255,255));
    }
    else if (isToday) {
        painter.setPen(skin->dayActive);
    }
    else {
        painter.setPen(isCurMonth ? skin->lunar : skin->lunarNotCurMonth);
    }
    painter.drawText(textRect, lunar, option);

    QRect r1 = rect();
    auto x = r1.right() - 11;
    auto y = r1.top() + 14;

    if (isActive) {
        painter.setPen(QColor(255,255,255));
    }
    else {
        painter.setPen(skin->dayWorking);
    }
    painter.drawText(QPoint(x, y), docStatus);

    if (hasSchdule) {
        QRect r2(textRect.width() / 2 - 2, textRect.bottom() - 6, 4, 4);        
        if (isActive) {
            painter.setBrush(QColor(255, 255, 255));
        }
        else {
            painter.setBrush(skin->dot);
        }
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(r2);
    }
}

void DayBtn::onClick()
{
    QString msg{ R"({"msgType":"EmbedCalendar","msgName":"changeDate","data":{"year":%1,"month":%2,"date":%3}})" };
    msg = msg.arg(year).arg(month).arg(date);
    WsConn::get()->sendMsg(msg);
}
