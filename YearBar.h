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
	void leftBtnClick();
	void rightBtnClick();
	void todayBtnClick();
	void btnLeave();
private:
	YearBarBtn* leftBtn;
	YearBarBtn* rightBtn;
	YearBarBtnToday* todayBtn;
};
