#include <QRect>
#include <QPainter>
#include "Skin.h"
#include "WeekBar.h"

WeekBar::WeekBar(QWidget *parent) : QWidget(parent)
{
	auto winWidth = parent->width();
	setGeometry(11, 100, winWidth-22, 28);
	list.append(QString::fromLocal8Bit("一"));
	list.append(QString::fromLocal8Bit("二"));
	list.append(QString::fromLocal8Bit("三"));
	list.append(QString::fromLocal8Bit("四"));
	list.append(QString::fromLocal8Bit("五"));
	list.append(QString::fromLocal8Bit("六"));
	list.append(QString::fromLocal8Bit("七"));
}

WeekBar::~WeekBar()
{
	
}

void WeekBar::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);
	QFont font("Microsoft YaHei", 10);
	font.setStyleStrategy(QFont::PreferAntialias);
	font.setHintingPreference(QFont::PreferNoHinting);
	painter.setFont(font);
	auto skin = Skin::get();
	painter.setPen(skin->week);
	auto w = width() / 7;
	for (size_t i = 0; i < 7; i++)
	{
		QRect r(i * w, 0, w, height());
		painter.drawText(r, Qt::AlignCenter, list[i]);
	}
}
