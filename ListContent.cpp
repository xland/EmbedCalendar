#include <QScrollBar>
#include <QJsonArray>

#include "MainWindow.h"
#include "WsConn.h"
#include "ListItem.h"
#include "ListContent.h"

ListContent* listContent;


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

   
}

void ListContent::updateData(const QJsonObject& obj)
{
    QWidget* contentWidget = widget();
    if (contentWidget) {
        delete contentWidget;
    }
    contentWidget = new QWidget(this);
    contentWidget->setFixedWidth(this->width());
    contentWidget->setStyleSheet(R"(background:transparent;margin:0px;padding:0px;)");
    layout = new QVBoxLayout(contentWidget);
    layout->setMargin(0);
    layout->setSpacing(8);
    auto arr = obj["scheduleList"].toArray();
    for (int i = 0; i < arr.size(); ++i) {
        auto item = new ListItem(contentWidget);
        auto data = arr[i].toObject();
        item->editTip = obj["lang"].toObject()["editSchedule"].toString();
        item->delTip = obj["lang"].toObject()["deleteSchedule"].toString();
        item->setData(data);
        layout->addWidget(item);
    }
    layout->addStretch();
    setWidget(contentWidget);
}

ListContent::~ListContent()
{
    listContent = nullptr;
}

void ListContent::scroll(const int& dis)
{
    auto bar = verticalScrollBar();
    if (!bar->isVisible()) return;
    auto win = MainWindow::get();
    auto pos = win->mapFromGlobal(QCursor::pos());
    auto r = geometry();
    if (r.contains(pos)) {        
        bar->setValue(bar->value() + (0 - dis / 3));
    }    
}

void ListContent::init()
{
    connect(WsConn::get(), &WsConn::onData, [](const QJsonObject& obj) {
        if (!listContent) {
            listContent = new ListContent(MainWindow::get());
        }
        if (obj["displayScheduleList"].toBool()) {
            listContent->updateData(obj);
            listContent->show();
        }
        else {
            listContent->hide();
        }
        });
}

ListContent* ListContent::get()
{
    return listContent;
}
