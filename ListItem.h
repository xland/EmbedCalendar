#pragma once
#include <QJsonArray>
#include <QJsonObject>
#include <QWidget>
#include "BtnBase.h"

class ListItemBtn;
class ListItemCheckBtn;
class ListItem : public BtnBase
{
	Q_OBJECT

public:
	ListItem(QWidget *parent = nullptr);
	~ListItem();
	void setData(const QJsonObject& data);
	virtual void enterEvent(QEvent* event) override;
	virtual void leaveEvent(QEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	
	QString editTip;
	QString delTip;
	QString clickToCompleteTodo;
	QString clickToRestartTodo;
	QColor calendarColor;
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	void enter();
	void leave();
	void enterCheck();
	void enterEdit();
	void enterDel();
	void clickCheck();
	void clickEdit();
	void clickDel();
	void leaveBtn();
private:
	ListItemBtn* editBtn;
	ListItemBtn* delBtn;
	ListItemCheckBtn* checkBtn;
	QString title;
	QString desc;
	QString scheduleNo;
	QString calendarNo;
	bool isAllowEdit;
	bool isAllowView;
};
