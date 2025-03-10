﻿#include <QRect>
#include <QPainter>
#include "Skin.h"
#include "Util.h"
#include "ListBar.h"
#include "ListContent.h"
#include "MainWindow.h"
#include "SwitchBar.h"

SwitchBar::SwitchBar(QWidget *parent) : QWidget(parent)
{
	setGeometry(260, parent->height()-60, 104, 60);
	setCursor(Qt::CursorShape::PointingHandCursor);
}

SwitchBar::~SwitchBar()
{
	
}

void SwitchBar::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	auto skin = Skin::get();
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);
	auto font = Util::getTextFont(16);
	painter.setFont(*font);
	painter.setBrush(Qt::NoBrush);
	painter.setPen(skin->switchText);
	auto flag = window()->height() > 480;
	QString text = QString::fromLocal8Bit("隐藏日程");
	QChar code(0xe708);
	if (!flag) {
		text = QString::fromLocal8Bit("显示日程");
		code = QChar(0xe70f);
	}
	painter.drawText(QPoint(8,36), text);
	auto fontIcon = Util::getIconFont(16);
	painter.setFont(*fontIcon);
	painter.drawText(QPoint(76, 36), code);
}

void SwitchBar::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) 
	{
		auto win = (MainWindow*)window();
		auto flag = win->height() > 480;
		if (flag) {
			win->setFixedHeight(480);
			win->listBar->hide();
			win->listContent->hide();
		}
		else {
			win->setFixedHeight(730);
			win->listBar->show();
			win->listContent->show();
		}
		setGeometry(260, win->height() - 60, 104, 60);
	}
}
