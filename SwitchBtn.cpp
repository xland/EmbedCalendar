#include <QRect>
#include <QPainter>

#include "WsConn.h"
#include "Skin.h"
#include "Util.h"
#include "ListBar.h"
#include "ListContent.h"
#include "MainWindow.h"
#include "SwitchBtn.h"

SwitchBtn::SwitchBtn(QWidget *parent) : BtnBase(parent)
{
	setGeometry(260, parent->height()-60, 104, 60);
}

SwitchBtn::~SwitchBtn()
{
	
}

void SwitchBtn::paintEvent(QPaintEvent* event)
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
	auto fontIcon = Util::getIconFont(16);
	if (flag) {
		auto str = WsConn::get()->data["lang"].toObject()["hideSchedule"].toString();
		QChar code(0xe708);
		painter.drawText(QPoint(8, 36), str);
		painter.setFont(*fontIcon);
		painter.drawText(QPoint(76, 36), code);
	}
	else {
		auto str = WsConn::get()->data["lang"].toObject()["displaySchedule"].toString();
		QChar code = QChar(0xe70f);
		painter.drawText(QPoint(8, 36), str);
		painter.setFont(*fontIcon);
		painter.drawText(QPoint(76, 36), code);
	}	
}

void SwitchBtn::mousePressEvent(QMouseEvent* event)
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
