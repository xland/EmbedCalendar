#include "ListItem.h"
#include "ListContent.h"

ListContent::ListContent(QWidget *parent) : QScrollArea(parent)
{
    setFrameShape(QFrame::NoFrame);
    setGeometry(20, 474, parent->width() - 40, 730 - 474 - 56);

    QWidget* contentWidget = new QWidget(this);
    contentWidget->setFixedWidth(parent->width() - 46);
    contentWidget->setStyleSheet(R"(background: transparent;)");
    QVBoxLayout* layout = new QVBoxLayout(contentWidget);
    for (int i = 0; i < 20; ++i) {
        auto item = new ListItem(this);
        layout->addWidget(item);
    }
    layout->addStretch();
    setWidget(contentWidget);

    setStyleSheet(R"(
QScrollArea {
    border: none;
    background: transparent;
}
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


}

ListContent::~ListContent()
{
	
}
