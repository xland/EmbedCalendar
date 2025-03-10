#pragma once

#include <QWidget>

class TipInfo : public QWidget
{
	Q_OBJECT

public:
	TipInfo(QWidget *parent = nullptr);
	~TipInfo();
	void showInfo(const QString& text, const QPoint& pos);
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	QString text;
};
