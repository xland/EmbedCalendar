#include <QRect>
#include <QPainter>

#include "WsConn.h"
#include "MainWindow.h"
#include "TipInfo.h"
#include "Util.h"
#include "Skin.h"
#include "YearBar.h"

YearBar::YearBar(QWidget *parent) : QWidget(parent)
{
	setGeometry(0, 48, 372, 44);
	leftBtn = new YearBarBtn(0xe709,this);
	leftBtn->move(96, 12);
	rightBtn = new YearBarBtn(0xe70e,this);
	rightBtn->move(256, 12);
	todayBtn = new YearBarBtnToday(this);
	todayBtn->move(335, 12);
	connect(leftBtn, &YearBarBtn::enter, this, &YearBar::leftBtnEnter);
	connect(rightBtn, &YearBarBtn::enter, this, &YearBar::rightBtnEnter);
	connect(todayBtn, &YearBarBtn::enter, this, &YearBar::todayBtnEnter);


	connect(leftBtn, &YearBarBtn::click, this, &YearBar::leftBtnClick);
	connect(rightBtn, &YearBarBtn::click, this, &YearBar::rightBtnClick);
	connect(todayBtn, &YearBarBtn::click, this, &YearBar::todayBtnClick);

	connect(leftBtn, &YearBarBtn::leave, this, &YearBar::btnLeave);
	connect(rightBtn, &YearBarBtn::leave, this, &YearBar::btnLeave);
	connect(todayBtn, &YearBarBtn::leave, this, &YearBar::btnLeave);
}

YearBar::~YearBar()
{
	
}

void YearBar::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);
	auto font = Util::getTextFont(20);
	painter.setFont(*font);
	auto skin = Skin::get();
	painter.setPen(skin->year);
	auto str = WsConn::get()->data["activeDateMonth"].toString();
	painter.drawText(rect(), Qt::AlignCenter, str);
}

void YearBar::leftBtnEnter()
{
	auto win = (MainWindow*)window();
	auto str = WsConn::get()->data["lang"].toObject()["prevMonth"].toString();
	win->tipInfo->setText(str);
	win->tipInfo->showInfo(QPoint(78, 26));
}

void YearBar::rightBtnEnter()
{
	auto win = (MainWindow*)window();
	auto str = WsConn::get()->data["lang"].toObject()["nextMonth"].toString();
	win->tipInfo->setText(str);
	win->tipInfo->showInfo(QPoint(238, 26));
}

void YearBar::todayBtnEnter()
{
	auto win = (MainWindow*)window();
	//auto str = WsConn::get()->data["lang"].toObject()["embed"].toString();
	win->tipInfo->setText(QString::fromLocal8Bit("今天"));
	win->tipInfo->showInfo(QPoint(324,26));
}

void YearBar::leftBtnClick()
{
	QString msg{ R"({"msgType":"EmbedCalendar","msgName":"changePrevMonth"})" };
	WsConn::get()->sendMsg(msg);
}

void YearBar::rightBtnClick()
{
	QString msg{ R"({"msgType":"EmbedCalendar","msgName":"changeNextMonth"})" };
	WsConn::get()->sendMsg(msg);
}

void YearBar::todayBtnClick()
{
	QString msg{ R"({"msgType":"EmbedCalendar","msgName":"changeToday"})" };
	WsConn::get()->sendMsg(msg);
}

void YearBar::btnLeave()
{
	auto win = (MainWindow*)window();
	win->tipInfo->hide();
}
