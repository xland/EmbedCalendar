#pragma once
#include <QWidget>
#include <QPainter>

#include "BtnBase.h"

class MenuItem : public BtnBase
{
	Q_OBJECT
public:
	MenuItem(QWidget *parent = nullptr);
	~MenuItem();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
};
