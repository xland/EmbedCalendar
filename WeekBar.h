#pragma once
#include <QMouseEvent>
#include <QWidget>
#include <QList>
#include <QString>
#include "YearBarBtn.h"

class WeekBar : public QWidget
{
	Q_OBJECT
public:
	WeekBar(QWidget *parent = nullptr);
	~WeekBar();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	QList<QString> list;
};
