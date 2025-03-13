#include "Menu.h"
#include "WsConn.h"
#include "MainWindow.h"

Menu* menu;

Menu::Menu(QWidget *parent) : QWidget(parent)
{
	menuIcon.append(QChar(0xe710));
	menuIcon.append(QChar(0xe70a));
	menuIcon.append(QChar(0xe711));
	menuIcon.append(QChar(0xe70d));
}

Menu::~Menu()
{
}

void Menu::init()
{
	connect(WsConn::get(), &WsConn::onData, [](const QJsonObject& obj) {
		if (!menu) {
			menu = new Menu(MainWindow::get());
		}
		menu->menuText.clear();
		auto lang = obj["lang"].toObject();
		menu->menuText.append(lang["setting"].toString());
		menu->menuText.append(lang["help"].toString());
		menu->menuText.append(lang["advise"].toString());
		menu->menuText.append(lang["signout"].toString());
	});
}

Menu* Menu::get()
{
	return menu;
}
