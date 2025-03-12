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
	~WeekBar();
	static void init();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	WeekBar(QWidget* parent = nullptr);
	QList<QString> list;
};
