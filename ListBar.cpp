#include <QRect>
#include <QPainter>
#include "Skin.h"
#include "Util.h"
#include "ListBar.h"

ListBar::ListBar(QWidget *parent) : QWidget(parent)
{
	setGeometry(20, 436, parent->width()-40, 26);
	setCursor(Qt::CursorShape::PointingHandCursor);
}

ListBar::~ListBar()
{
	
}

void ListBar::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	auto skin = Skin::get();
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);
	QFont font("Microsoft YaHei", 14);
	font.setStyleStrategy(QFont::PreferAntialias);
	font.setHintingPreference(QFont::PreferNoHinting);
	painter.setFont(font);
	painter.setBrush(Qt::NoBrush);
	painter.setPen(skin->switchText);
	painter.drawText(rect(), Qt::AlignVCenter, QString::fromLocal8Bit("今天 七月廿一"));
}
