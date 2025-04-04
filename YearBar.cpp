﻿#include <QRect>
#include <QPainter>

#include "Menu.h"
#include "WsConn.h"
#include "MainWindow.h"
#include "TipInfo.h"
#include "Util.h"
#include "Skin.h"
#include "YearBar.h"
#include "Toast.h"

YearBar* yearBar;


YearBar::YearBar(QWidget *parent) : QWidget(parent)
{
	setGeometry(0, 48, 372, 44);
	leftBtn = new YearBarBtn(0xe709,this);
	leftBtn->move(80, 12);
	rightBtn = new YearBarBtn(0xe70e,this);
	rightBtn->move(240, 12);
	todayBtn = new YearBarBtnToday(this);
	todayBtn->move(308, 12);
	connect(leftBtn, &YearBarBtn::enter, this, &YearBar::leftBtnEnter);
	connect(rightBtn, &YearBarBtn::enter, this, &YearBar::rightBtnEnter);

	connect(leftBtn, &YearBarBtn::click, this, &YearBar::leftBtnClick);
	connect(rightBtn, &YearBarBtn::click, this, &YearBar::rightBtnClick);
	connect(todayBtn, &YearBarBtn::click, this, &YearBar::todayBtnClick);

	connect(leftBtn, &YearBarBtn::leave, this, &YearBar::btnLeave);
	connect(rightBtn, &YearBarBtn::leave, this, &YearBar::btnLeave);
	connect(todayBtn, &YearBarBtn::leave, this, &YearBar::btnLeave);
}

YearBar::~YearBar()
{
	yearBar = nullptr;
}

void YearBar::init()
{
	connect(WsConn::get(), &WsConn::onData, [](const QJsonObject& obj) {
		if (!yearBar) {
			yearBar = new YearBar(MainWindow::get());
		}
		auto lang = obj["lang"].toObject();
		yearBar->leftTip = lang["prevMonth"].toString();
		yearBar->rightTip = lang["nextMonth"].toString();
		yearBar->activeDateMonth = obj["activeDateMonth"].toString();
		yearBar->todayBtn->isCn = obj["isCn"].toBool();
		yearBar->show();
		});
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
	painter.drawText(rect(), Qt::AlignCenter, activeDateMonth);
}

void YearBar::leftBtnEnter()
{
	auto tipObj = TipInfo::get();
	tipObj->setText(leftTip);
	tipObj->showInfo(QPoint(78, 26));
}

void YearBar::rightBtnEnter()
{
	if (Menu::get()->isVisible()) return;
	auto tipObj = TipInfo::get();
	tipObj->setText(rightTip);
	tipObj->showInfo(QPoint(238, 26));
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
	//Toast::start("error","asdf asdf");
}

void YearBar::btnLeave()
{
	TipInfo::get()->hide();
}
