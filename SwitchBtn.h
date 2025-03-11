#pragma once
#include <QMouseEvent>
#include <QWidget>
#include "BtnBase.h"

class SwitchBtn : public BtnBase
{
	Q_OBJECT
public:
	SwitchBtn(QWidget *parent = nullptr);
	~SwitchBtn();
protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
private:
};
