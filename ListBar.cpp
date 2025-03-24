#include <QRect>
#include <QPainter>
#include "MainWindow.h"
#include "TipInfo.h"
#include "Skin.h"
#include "Util.h"
#include "WsConn.h"
#include "ListBar.h"
#include "ListBarBtn.h"

ListBar* listBar;


ListBar::ListBar(QWidget *parent) : QWidget(parent)
{
	setGeometry(20, 436, parent->width()-40, 26);
	btn = new ListBarBtn(this);
	connect(btn, &ListBarBtn::enter, this, &ListBar::btnEnter);
	connect(btn, &ListBarBtn::leave, this, &ListBar::btnLeave);
	connect(btn, &ListBarBtn::click, this, &ListBar::btnClick);
}

ListBar::~ListBar()
{
	listBar = nullptr;
}

void ListBar::init()
{
	connect(WsConn::get(), &WsConn::onData, [](const QJsonObject& obj) {
		if (!listBar) {
			listBar = new ListBar(MainWindow::get());
		}
		auto lang = obj["lang"].toObject();
		listBar->activeDateDay = obj["activeDateDay"].toString();
		listBar->tipInfo = lang["createSchedule"].toString();
		if (obj["displayScheduleList"].toBool()) {
			listBar->show();
		}
		else {
			listBar->hide();
		}		
	});
}

void ListBar::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	auto skin = Skin::get();
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);
	auto font = Util::getTextFont(18);
	painter.setFont(*font);
	painter.setBrush(Qt::NoBrush);
	painter.setPen(skin->listHeader);
	painter.drawText(rect(), Qt::AlignVCenter, activeDateDay);
}

void ListBar::btnEnter()
{
	if (window()->height() > 480) {
		auto tipObj = TipInfo::get();
		tipObj->setText(tipInfo);
		auto left = btn->pos().x() - tipObj->width() + 18;
		tipObj->showInfo(QPoint(left, 432));
	}
}

void ListBar::btnLeave()
{
	auto tipObj = TipInfo::get();
	tipObj->hide();
}

void ListBar::btnClick()
{
	QString msg{ R"({"msgType":"EmbedCalendar","msgName":"createSchedule"})" };
	WsConn::get()->sendMsg(msg);
}
