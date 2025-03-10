#include <QRect>
#include <QPainter>

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
	painter.drawText(rect(), Qt::AlignCenter, QString::fromLocal8Bit("2024年8月"));
}

void YearBar::leftBtnEnter()
{
	auto win = (MainWindow*)window();
	win->tipInfo->showInfo(QString::fromLocal8Bit("上个月"), QPoint(78, 26));
}

void YearBar::rightBtnEnter()
{
	auto win = (MainWindow*)window();
	win->tipInfo->showInfo(QString::fromLocal8Bit("下个月"), QPoint(238, 26));
}

void YearBar::todayBtnEnter()
{
	auto win = (MainWindow*)window();
	win->tipInfo->showInfo(QString::fromLocal8Bit("今天"), QPoint(324,26));
}
