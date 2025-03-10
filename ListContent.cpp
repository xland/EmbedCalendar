#include "ListItem.h"
#include "ListContent.h"

ListContent::ListContent(QWidget *parent) : QScrollArea(parent)
{
    setFrameShape(QFrame::NoFrame);
    setGeometry(20, 474, parent->width() - 40, 730 - 474 - 56);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet(R"(QScrollArea{border: none; background: transparent;}
QScrollBar:vertical {
    border: none;
    background: transparent; /* 滚动条背景颜色 */
    width: 6px;         /* 滚动条宽度 */
    margin: 0px 0px 0px 0px;
}
QScrollBar::handle:vertical {
    background: #dddddd; /* 滑块颜色 */
    min-height: 20px;    /* 滑块最小高度 */
    border-radius: 2px;  /* 圆角 */
}
QScrollBar::handle:vertical:hover {
    background: #cccccc; /* 鼠标悬停时的滑块颜色 */
}
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;         /* 移除上下箭头 */
    background: none;
}
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
    background: none;     /* 滑块上下区域的背景 */
})");

    QWidget* contentWidget = new QWidget(this);
    contentWidget->setStyleSheet(R"(background:transparent;margin:0px;padding:0px;)");
    QVBoxLayout* layout = new QVBoxLayout(contentWidget);
    layout->setMargin(0);
    layout->setSpacing(8);
    for (int i = 0; i < 13; ++i) {
        auto item = new ListItem(this);
        layout->addWidget(item);
    }
    layout->addStretch();
    setWidget(contentWidget);
}

ListContent::~ListContent()
{
	
}
