#pragma once
#include <QJsonArray>
#include <QJsonObject>
#include <QWidget>
#include "BtnBase.h"

class ListItemBtn;
class ListItem : public BtnBase
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
private:
	void enterEdit();
	void enterDel();
	void leaveBtn();
private:
	ListItemBtn* editBtn;
	ListItemBtn* delBtn;
};
