#include "MenuItem.h"

MenuItem::MenuItem(const QChar& icon, const QString& text, QWidget *parent) : BtnBase(parent),
icon{ icon }, text{text}
{

}

MenuItem::~MenuItem()
{
	
}

void MenuItem::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
}
