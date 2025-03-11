#include <QPaintEvent>
#include <QPainter>

#include "MainWindow.h"
#include "TipInfo.h"
#include "Skin.h"
#include "Util.h"
#include "TitleBarBtn.h"

TitleBarBtn::TitleBarBtn(const uint& code, QWidget* parent) : BtnBase(parent), code{code}
{
    setFixedSize(28, 28);
}

TitleBarBtn::~TitleBarBtn()
{
	
}

void TitleBarBtn::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto skin = Skin::get();
    if (isHover) {
        painter.setBrush(skin->titleBtnHover);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(rect(), 2, 2);
    }
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    auto font = Util::getIconFont(18);
    painter.setFont(*font);
    painter.setPen(skin->titleBtn);
    painter.drawText(rect(), Qt::AlignCenter, QChar(code));
}
