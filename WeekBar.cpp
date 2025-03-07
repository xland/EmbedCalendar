#include <QRect>
#include <QPainter>
#include "WeekBar.h"

WeekBar::WeekBar(QWidget *parent) : QWidget(parent)
{
	//570
	setGeometry(20, 108, 530, 28);
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
	painter.setBrush(QColor(180, 180, 180, 180));
	painter.drawRect(rect());
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);
	QFont font("Microsoft YaHei", 18);
	font.setStyleStrategy(QFont::PreferAntialias);
	font.setHintingPreference(QFont::PreferNoHinting);
	painter.setFont(font);
	painter.setPen(QColor(210, 211, 212));
	auto w = width() / 7;
	for (size_t i = 0; i < 7; i++)
	{
		QRect r(i * w, 0, w, height());
		painter.drawText(r, Qt::AlignCenter, list[i]);
	}
}
