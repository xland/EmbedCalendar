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
	void setData(const QJsonObject& data);
	
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
	QString title;
	QString desc;
	QColor calendarColor;
	bool isAllowEdit;
};
