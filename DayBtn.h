#pragma once
#include <QMouseEvent>
#include <QWidget>
#include <QJsonObject>
#include "BtnBase.h"

class DayBtn : public BtnBase
{
	Q_OBJECT
public:
	DayBtn(const int& index,QWidget *parent = nullptr);
	~DayBtn();
	void updateData(const QJsonObject& obj);
public:
	bool isCn{ true };
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	void onClick();
private:
	int index;
	int year;
	int month;
	int date;
	QString lunar;
	QString docStatus;
	bool hasSchdule;
	bool isActive;
	bool isToday;
	bool isCurMonth;
};
