#pragma once

#include <QWidget>
#include "BtnBase.h"

class ListItemCheckBtn : public BtnBase
{
	Q_OBJECT

public:
	ListItemCheckBtn(QWidget *parent = nullptr);
	~ListItemCheckBtn();
	int taskProcess;
protected:
	void paintEvent(QPaintEvent* event) override;
private:
};
