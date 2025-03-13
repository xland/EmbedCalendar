#include <QRect>
#include "MainWindow.h"
#include "TipInfo.h"
#include "TitleBar.h"
#include "Menu.h"
#include "WsConn.h"

TitleBar* titleBar;


TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
	auto winWidth = parent->width();
	setGeometry(0, 0, winWidth, 48);
	pinBtn = new TitleBarBtn(0xe70c,this);
	pinBtn->move(winWidth - 80, 10);
	menuBtn = new TitleBarBtn(0xe6e8, this);
	menuBtn->move(winWidth - 45, 10);
	connect(menuBtn, &TitleBarBtn::enter, this, &TitleBar::menuBtnEnter);
	connect(menuBtn, &TitleBarBtn::leave, this, &TitleBar::menuBtnLeave);
	connect(pinBtn, &TitleBarBtn::enter, this, &TitleBar::pinBtnEnter);
	connect(pinBtn, &TitleBarBtn::leave, this, &TitleBar::pinBtnLeave);
	connect(pinBtn, &TitleBarBtn::click, this, &TitleBar::pinBtnClick);	

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

TitleBar* TitleBar::get()
{
	return titleBar;
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

void TitleBar::menuBtnEnter()
{
	auto menu = Menu::get();
	menu->raise();
	menu->show();
}

void TitleBar::menuBtnLeave()
{
	if (!menuBtn->isHover && !Menu::get()->isHover) {
		Menu::get()->hide();
	}
}

void TitleBar::pinBtnClick()
{
	auto win = MainWindow::get();
	win->switchEmbed();
}

void TitleBar::pinBtnEnter()
{
	auto tipObj = TipInfo::get();
	tipObj->setText(tipInfo);
	tipObj->showInfo(QPoint(width()-128, 8));
}

void TitleBar::pinBtnLeave()
{
	TipInfo::get()->hide();
}
