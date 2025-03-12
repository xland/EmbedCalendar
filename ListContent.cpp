#include <QScrollBar>
#include <QJsonArray>

#include "WsConn.h"
#include "ListItem.h"
#include "ListContent.h"

ListContent::ListContent(QWidget *parent) : QScrollArea(parent)
{
    if (parent->height() < 730) {
        setVisible(false);
    }
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

    auto arr = WsConn::get()->data["scheduleList"].toArray();
    for (int i = 0; i < arr.size(); ++i) {
        auto obj = arr[i].toObject();
        auto item = new ListItem(obj,this);
        layout->addWidget(item);
    }
    layout->addStretch();
    setWidget(contentWidget);
}

ListContent::~ListContent()
{
	
}

void ListContent::scroll(const int& dis)
{
    //scrollContentsBy(0, dis/3);
    auto bar = verticalScrollBar();
    bar->setValue(bar->value()+(0- dis / 3));
}
