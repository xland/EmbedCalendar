#pragma once
#include <QJsonArray>
#include <QJsonObject>
#include <QWidget>
class ListItemBtn;
class ListItem : public QWidget
{
	Q_OBJECT

public:
	ListItem(QWidget *parent = nullptr);
	~ListItem();
	QString title;
	QString desc;
	QColor calendarColor;
	QString editTip;
	QString delTip;
protected:
	void paintEvent(QPaintEvent* event) override;
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;
private:
	void enterEdit();
	void enterDel();
	void leaveBtn();
private:
	ListItemBtn* editBtn;
	ListItemBtn* delBtn;
	bool isHover{ false };
};
