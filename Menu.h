#pragma once

#include <QWidget>

class Menu : public QWidget
{
	Q_OBJECT

public:
	Menu(QWidget *parent = nullptr);
	~Menu();
	static void init();
	static Menu* get();
private:
	QList<QString> menuText;
	QList<QChar> menuIcon;
};
