#pragma once

#include <QWidget>
#include <QJsonObject>
#include "MenuItem.h"
#include "BtnBase.h"

class Menu : public BtnBase
{
	Q_OBJECT

public:
	Menu(QWidget *parent = nullptr);
	~Menu();
	static void init();
	static Menu* get();
	void setData(const QJsonObject& obj);
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	void mouseLeave();
	MenuItem* item0;
	MenuItem* item1;
	MenuItem* item2;
	MenuItem* item3;
	QList<QString> menuText;
	QList<QChar> menuIcon;
};
