#include <QPainter>

#include "Skin.h"
#include "TipInfo.h"

TipInfo::TipInfo(QWidget *parent) : QWidget(parent)
{
}

TipInfo::~TipInfo()
{
	
}

void TipInfo::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto skin = Skin::get();
    painter.setBrush(skin->tipInfoBg);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 4, 4);
}
