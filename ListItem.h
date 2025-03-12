#pragma once

#include <QJsonObject>
#include <QWidget>
class ListItemBtn;
class ListItem : public QWidget
{
	Q_OBJECT

public:
	ListItem(const QJsonObject& obj, QWidget *parent = nullptr);
	~ListItem();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	void enterEdit();
	void enterDel();
	void leaveBtn();
private:
	ListItemBtn* editBtn;
	ListItemBtn* delBtn;
	QString title;
	QString desc;
	bool isAllowEdit;
	QColor calendarColor;
	QString calendarNo;
	QString scheduleNo;
};
