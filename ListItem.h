#pragma once

#include <QWidget>

class ListItem : public QWidget
{
	Q_OBJECT

public:
	ListItem(QWidget *parent = nullptr);
	~ListItem();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
};
