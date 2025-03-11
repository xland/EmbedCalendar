#pragma once
#include <QMouseEvent>
#include <QWidget>
#include "BtnBase.h"

class ListBarBtn : public BtnBase
{
	Q_OBJECT
public:
	ListBarBtn(QWidget *parent = nullptr);
	~ListBarBtn();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
};
