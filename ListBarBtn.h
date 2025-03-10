﻿#pragma once
#include <QMouseEvent>
#include <QWidget>

class ListBarBtn : public QWidget
{
	Q_OBJECT
public:
	ListBarBtn(QWidget *parent = nullptr);
	~ListBarBtn();
protected:
	void paintEvent(QPaintEvent* event) override;
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
private:
	bool isHover{ false };
};
