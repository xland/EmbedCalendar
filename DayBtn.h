#pragma once
#include <QMouseEvent>
#include <QWidget>
#include "BtnBase.h"

class DayBtn : public BtnBase
{
	Q_OBJECT
public:
	DayBtn(const int& index,QWidget *parent = nullptr);
	~DayBtn();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	int index;
};
