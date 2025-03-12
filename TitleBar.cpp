#include <QRect>
#include "MainWindow.h"
#include "TipInfo.h"
#include "TitleBar.h"
#include "WsConn.h"

TitleBar* titleBar;


TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
	auto winWidth = parent->width();
	setGeometry(0, 0, winWidth, 48);
	pinBtn = new TitleBarBtn(0xe70c,this);
	pinBtn->move(winWidth - 80, 10);
	connect(pinBtn, &TitleBarBtn::enter, this, &TitleBar::enterPinBtn);
	connect(pinBtn, &TitleBarBtn::leave, this, &TitleBar::leavePinBtn);
	connect(pinBtn, &TitleBarBtn::click, this, &TitleBar::pinBtnClick);	
	menuBtn = new TitleBarBtn(0xe6e8,this);
	menuBtn->move(winWidth - 45, 10);
}

TitleBar::~TitleBar()
{
	titleBar = nullptr;
}

void TitleBar::init()
{
	connect(WsConn::get(), &WsConn::onData, [](const QJsonObject& obj) {
		if (!titleBar) {
			titleBar = new TitleBar(MainWindow::get());
		}
		titleBar->tipInfo = obj["lang"].toObject()["embed"].toString();
		titleBar->show();
	});
}

void TitleBar::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) 
	{
		auto pos = event->pos();
		if (pinBtn->rect().contains(pos) || menuBtn->rect().contains(pos)) {
			return;
		}
		pressPos = pos;
		isDragging = true;
	}
}

void TitleBar::mouseMoveEvent(QMouseEvent* event)
{
	if (isDragging) {
		QPoint movePot = event->globalPos() - pressPos;
		window()->move(movePot);
	}
}

void TitleBar::mouseReleaseEvent(QMouseEvent* event)
{
	if (isDragging) {
		auto win = MainWindow::get();
		auto pos = win->pos();
		QString msg{ R"({"msgType":"EmbedCalendar","msgName":"embedWin","data":{"hasEmbed":%1,"x":%2,"y":%3}})" };
		msg = msg.arg("false").arg(pos.x()).arg(pos.y());
		WsConn::get()->sendMsg(msg);
		isDragging = false;
	}
}

void TitleBar::pinBtnClick()
{
	auto win = MainWindow::get();
	win->switchEmbed();
}

void TitleBar::enterPinBtn()
{
	auto win = MainWindow::get();
	win->tipInfo->setText(tipInfo);
	win->tipInfo->showInfo(QPoint(width()-128, 8));
}

void TitleBar::leavePinBtn()
{
	auto win = MainWindow::get();
	win->tipInfo->hide();
}
