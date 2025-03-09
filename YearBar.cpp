#include <QRect>
#include <QPainter>
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
}

YearBar::~YearBar()
{
	
}

void YearBar::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);
	QFont font("Microsoft YaHei", 16);
	font.setStyleStrategy(QFont::PreferAntialias);
	font.setHintingPreference(QFont::PreferNoHinting);
	painter.setFont(font);
	auto skin = Skin::get();
	painter.setPen(skin->year);
	painter.drawText(rect(), Qt::AlignCenter, QString::fromLocal8Bit("2024年8月"));
}
