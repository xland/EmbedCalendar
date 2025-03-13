#include <QVBoxLayout>
#include <QMouseEvent>
#include "TitleBar.h"
#include "TitleBarBtn.h"
#include "Menu.h"
#include "Skin.h"
#include "WsConn.h"
#include "MainWindow.h"

Menu* menu;

Menu::Menu(QWidget *parent) : BtnBase(parent)
{
	setGeometry(parent->width() - 124, 37, 108, 136);
	installEventFilter(this);
	connect(this, &BtnBase::leave, this, &Menu::mouseLeave);
}

Menu::~Menu()
{
	menu = nullptr;
}

void Menu::init()
{
	connect(WsConn::get(), &WsConn::onData, [](const QJsonObject& obj) {
		if (!menu) {
			menu = new Menu(MainWindow::get());
			menu->setData(obj);
		}
	});
}

Menu* Menu::get()
{
	return menu;
}

void Menu::setData(const QJsonObject& obj)
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setSpacing(10);
	layout->setContentsMargins(4, 4, 4, 4);
	auto lang = obj["lang"].toObject();
	layout->addWidget(new MenuItem(QChar(0xe710), lang["setting"].toString(), menu));
	layout->addWidget(new MenuItem(QChar(0xe70a), lang["help"].toString(), menu));
	layout->addWidget(new MenuItem(QChar(0xe711), lang["advise"].toString(), menu));
	layout->addWidget(new MenuItem(QChar(0xe70d), lang["signout"].toString(), menu));
	setLayout(layout);
}

void Menu::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	auto skin = Skin::get();
	painter.setBrush(skin->menuBg);
	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect(rect(), 4, 4);
}

void Menu::mouseLeave()
{
	if (!isHover && !TitleBar::get()->menuBtn->isHover) {
		menu->hide();
	}
}
