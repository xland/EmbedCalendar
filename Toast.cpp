#include <QPainter>
#include <QTimer>
#include <QDateTime>
#include "Toast.h"
#include "Util.h"
#include "MainWindow.h"

Toast* toast;

Toast::Toast(QWidget *parent) : QWidget(parent)
{
	setVisible(false);
	QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
	shadow->setOffset(0, 0);
	shadow->setColor(QColor(0, 0, 0, 60));
	shadow->setBlurRadius(12);
	setGraphicsEffect(shadow);
}

Toast::~Toast()
{
	toast = nullptr;
}

void Toast::start(const QString& type, const QString& text)
{
	if (!toast) {
		toast = new Toast(MainWindow::get());
	}
	if (toast->isVisible()) {
		toast->hide();
	}
	if (type == "success")
	{
		toast->color.setRgb(68, 159, 45);
		toast->code = 0xe74e;
	}
	else if (type == "error")
	{
		toast->color.setRgb(224, 54, 59);
		toast->code = 0xe74d;
	}
	else if (type == "warning")
	{
		toast->color.setRgb(245, 169, 43);
		toast->code = 0xe74f;
	}
	toast->text = text;

	auto font = Util::getTextFont(16);
	QFontMetrics metrics(*font);
	auto width = metrics.horizontalAdvance(text) + 30 + 14;
	auto height = 28;
	toast->setGeometry((toast->window()->width() - width) / 2, 18, width, height);
	toast->show();
	toast->showTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

}

void Toast::showEvent(QShowEvent* event)
{
	QTimer::singleShot(3000, [this]() {
		auto now = QDateTime::currentDateTime().toMSecsSinceEpoch();
		if (now - showTime < 2992) return;
		this->hide();
		});
}

void Toast::paintEvent(QPaintEvent* event)
{
	auto r0 = rect();
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	p.setRenderHint(QPainter::TextAntialiasing, true);
	//p.setBrush(QColor(0, 0, 0, 128));
	//p.setPen(Qt::NoPen);
	//p.drawRoundedRect(r0, 4, 4);

	auto font = Util::getTextFont(16);
	QFontMetrics metrics(*font);
	p.setFont(*font);
	p.setBrush(Qt::NoBrush);
	p.setPen(color); 
	p.setBrush(Qt::white);
	p.setPen(color);
	p.drawRoundedRect(rect().adjusted(1,1,-1,-1), 4, 4);
	p.drawText(32, 20, text);

	font = Util::getIconFont(16);
	p.setFont(*font);
	p.setBrush(Qt::NoBrush);
	p.setPen(color);
	p.drawText(10,20,QChar(code));
}
