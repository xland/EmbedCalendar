#include <QPaintEvent>
#include <QPainter>

#include "MainWindow.h"
#include "Skin.h"
#include "Util.h"
#include "TipInfo.h"
#include "ListItemBtn.h"

ListItemBtn::ListItemBtn(const uint& code, QWidget* parent) : BtnBase(parent), code{code}
{
    setFixedSize(26, 26);
}

ListItemBtn::~ListItemBtn()
{
	
}

void ListItemBtn::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto skin = Skin::get();
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    auto font = Util::getIconFont(14);
    painter.setFont(*font);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(skin->listItemBtn);
    painter.drawText(rect(), Qt::AlignCenter, QChar(code));
}
