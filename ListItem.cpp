#include <QPainter>
#include <QPainterPath>
#include "Skin.h"
#include "ListItem.h"

ListItem::ListItem(QWidget *parent) : QWidget(parent)
{
	setFixedSize(parent->width()-10,44);
}

ListItem::~ListItem()
{
}

void ListItem::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿，使边缘更平滑

    // 定义矩形区域
    QRectF rect(0, 0, 3, 44); // x, y, width, height
    qreal radius = 3.0; // 圆角半径

    // 创建 QPainterPath 用于自定义形状
    QPainterPath path;

    // 从左上角开始（逆时针绘制）
    path.moveTo(rect.left() + radius, rect.top()); // 移动到左上角的圆弧起点
    path.arcTo(rect.left(), rect.top(), radius * 2, radius * 2, 90, 90); // 左上圆角
    path.lineTo(rect.left(), rect.bottom() - radius); // 左侧直线
    path.arcTo(rect.left(), rect.bottom() - radius * 2, radius * 2, radius * 2, 180, 90); // 左下圆角
    path.lineTo(rect.right(), rect.bottom()); // 底部直线
    path.lineTo(rect.right(), rect.top()); // 右侧直线
    path.closeSubpath(); // 闭合路径

    // 设置画笔和填充
    painter.setPen(Qt::NoPen); // 边框颜色和宽度
    painter.setBrush(Qt::darkBlue); // 填充颜色
    painter.drawPath(path); // 绘制路径

    painter.setRenderHint(QPainter::TextAntialiasing, true);
    QFont font("Microsoft YaHei", 10);
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setHintingPreference(QFont::PreferNoHinting);
    painter.setFont(font);
    painter.setBrush(Qt::NoBrush);
    auto skin = Skin::get();
    painter.setPen(skin->listItemText1);
    painter.drawText(QPoint(8, 16), QString::fromLocal8Bit("这是日程的标题，这是日程的标题"));

    font.setPointSize(9);
    painter.setFont(font);
    painter.setPen(skin->listItemText2);
    painter.drawText(QPoint(8, 38), QString::fromLocal8Bit("12：12 这是日程的简介，这是日程的标题"));
}
