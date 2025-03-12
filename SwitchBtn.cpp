#include <QRect>
#include <QPainter>

#include "WsConn.h"
#include "Skin.h"
#include "Util.h"
#include "ListBar.h"
#include "ListContent.h"
#include "MainWindow.h"
#include "SwitchBtn.h"

SwitchBtn* switchBtn;

SwitchBtn::SwitchBtn(QWidget *parent) : BtnBase(parent)
{
	setGeometry(260, parent->height()-60, 104, 60);
}

SwitchBtn::~SwitchBtn()
{
	switchBtn = nullptr;
}
void SwitchBtn::init()
{
	connect(WsConn::get(), &WsConn::onData, [](const QJsonObject& obj) {
		if (!switchBtn) {
			switchBtn = new SwitchBtn(MainWindow::get());
		}
		auto lang = obj["lang"].toObject();
		switchBtn->hideSchedule = lang["hideSchedule"].toString();
		switchBtn->displaySchedule = lang["displaySchedule"].toString();
		switchBtn->move(260, MainWindow::get()->height() - 60);
		switchBtn->show();
		});
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
		QChar code(0xe708);
		painter.drawText(QPoint(8, 36), hideSchedule);
		painter.setFont(*fontIcon);
		painter.drawText(QPoint(76, 36), code);
	}
	else {
		QChar code = QChar(0xe70f);
		painter.drawText(QPoint(8, 36), displaySchedule);
		painter.setFont(*fontIcon);
		painter.drawText(QPoint(76, 36), code);
	}	
}

void SwitchBtn::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) 
	{
		auto win = MainWindow::get();
		auto flag = win->height() > 480;
		QString msg{ R"({"msgType":"EmbedCalendar","msgName":"displayScheduleList","data":%1})" };
		msg = msg.arg(flag ? "false" : "true");
		WsConn::get()->sendMsg(msg);
		msg = QString{ R"({"msgType":"EmbedCalendar","msgName":"updateRenderData"})" };
		WsConn::get()->sendMsg(msg);
	}
}
