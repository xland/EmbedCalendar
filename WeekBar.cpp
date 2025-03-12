#include <QRect>
#include <QPainter>
#include <QJsonArray>

#include "MainWindow.h"
#include "WsConn.h"
#include "Util.h"
#include "Skin.h"
#include "WeekBar.h"

WeekBar* weekBar;

WeekBar::WeekBar(QWidget *parent) : QWidget(parent)
{
	auto winWidth = parent->width();
	setGeometry(11, 100, winWidth-22, 28);
}

WeekBar::~WeekBar()
{
	weekBar = nullptr;
}

void WeekBar::init()
{
	connect(WsConn::get(), &WsConn::onData, [](const QJsonObject& obj) {
		if (!weekBar) {
			weekBar = new WeekBar(MainWindow::get());
		}
		weekBar->list.clear();
		auto arr = obj["weekLables"].toArray();
		for (int i = 0; i < arr.size(); i++)
		{
			weekBar->list.append(arr[i].toString());
		}
		weekBar->show();
		});
}
void WeekBar::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);
	auto font = Util::getTextFont(12);
	painter.setFont(*font);
	auto skin = Skin::get();
	painter.setPen(skin->week);
	auto w = width() / 7;
	for (size_t i = 0; i < 7; i++)
	{
		QRect r(i * w, 0, w, height());
		painter.drawText(r, Qt::AlignCenter, list[i]);
	}
}
