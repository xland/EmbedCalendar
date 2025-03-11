#pragma once
#include <QMouseEvent>
#include <QWidget>
#include <QJsonObject>
#include "BtnBase.h"

class DayBtn : public BtnBase
{
	Q_OBJECT
public:
	DayBtn(const int& index,const QJsonObject& obj,QWidget *parent = nullptr);
	~DayBtn();
public:
	QString day;
	QString lunar;
	QString docStatus;
	bool hasSchdule;
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	int index;
};
