#pragma once
#include <QMouseEvent>
#include <QWidget>
#include "BtnBase.h"

class ListItemBtn : public BtnBase
{
	Q_OBJECT
public:
	ListItemBtn(const uint& code, QWidget *parent = nullptr);
	~ListItemBtn();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	uint code;
};
