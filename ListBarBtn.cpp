#include <QPaintEvent>
#include <QPainter>
#include "Skin.h"
#include "Util.h"
#include "ListBarBtn.h"

ListBarBtn::ListBarBtn(QWidget* parent) : BtnBase(parent)
{
    setGeometry(parent->width()-26,0, 26, 26);
}

ListBarBtn::~ListBarBtn()
{
	
}

void ListBarBtn::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto skin = Skin::get();
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    auto font = Util::getIconFont(20);
    painter.setFont(*font);
    painter.setPen(skin->switchText);
    painter.drawText(rect(), Qt::AlignCenter, QChar(0xe70b));
}
