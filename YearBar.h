#pragma once
#include <QMouseEvent>
#include <QWidget>
#include "YearBarBtn.h"
#include "YearBarBtnToday.h"

class YearBar : public QWidget
{
	Q_OBJECT
public:
	YearBar(QWidget *parent = nullptr);
	~YearBar();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	void leftBtnEnter();
	void rightBtnEnter();
	void todayBtnEnter();
private:
	YearBarBtn* leftBtn;
	YearBarBtn* rightBtn;
	YearBarBtnToday* todayBtn;
};
