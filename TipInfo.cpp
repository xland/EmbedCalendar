#include <QPainter>

#include "Util.h"
#include "Skin.h"
#include "TipInfo.h"

TipInfo::TipInfo(QWidget *parent) : QWidget(parent)
{
    
    setVisible(false);
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

TipInfo::~TipInfo()
{
	
}

void TipInfo::showInfo(const QString& text,const QPoint& pos)
{
    this->text = text;
    auto font = Util::getTextFont(14);
    QFontMetrics fm(*font);
    int textAdvance = fm.horizontalAdvance(text);
    setFixedSize(textAdvance+18, 32);
    move(pos);
    this->show();
    this->raise();
}

void TipInfo::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto skin = Skin::get();
    painter.setBrush(skin->tipInfoBg);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 4, 4);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(skin->tipInfo);
    auto font = Util::getTextFont(14);
    painter.setFont(*font);
    painter.drawText(rect(), Qt::AlignCenter, text);
}
