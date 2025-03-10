#pragma once

#include <QWidget>

class TipInfo : public QWidget
{
	Q_OBJECT

public:
	TipInfo(QWidget *parent = nullptr);
	~TipInfo();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
};
