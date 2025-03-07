#include <QRect>
#include "TitleBar.h"

TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
	auto winWidth = parent->width();
	setGeometry(0, 0, winWidth, 48);
	pinBtn = new TitleBarBtn(0xe70c,this);
	pinBtn->move(winWidth - 80, 10);
	menuBtn = new TitleBarBtn(0xe6e8,this);
	menuBtn->move(winWidth - 45, 10);
}

TitleBar::~TitleBar()
{
	
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
	isDragging = false;
}
