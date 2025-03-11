#pragma once
#include <QMouseEvent>
#include <QWidget>
#include "BtnBase.h"

class TitleBarBtn : public BtnBase
{
	Q_OBJECT
public:
	TitleBarBtn(const uint& code, QWidget *parent = nullptr);
	~TitleBarBtn();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	uint code;
};
