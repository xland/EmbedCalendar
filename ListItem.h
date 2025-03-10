#pragma once

#include <QWidget>
class ListItemBtn;
class ListItem : public QWidget
{
	Q_OBJECT

public:
	ListItem(QWidget *parent = nullptr);
	~ListItem();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	void enterEdit();
	void enterDel();
private:
	ListItemBtn* editBtn;
	ListItemBtn* delBtn;
};
