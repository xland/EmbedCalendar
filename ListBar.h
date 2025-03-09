#pragma once
#include <QMouseEvent>
#include <QWidget>

class ListBar : public QWidget
{
	Q_OBJECT
public:
	ListBar(QWidget *parent = nullptr);
	~ListBar();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
};
