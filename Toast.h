#pragma once

#include <QWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class Toast : public QWidget
{
	Q_OBJECT

public:
	Toast(QWidget *parent = nullptr);
	~Toast();
	static void start(const QString& type, const QString& text);
protected:
	void showEvent(QShowEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
private:
	QString text;
	QColor color;
	ushort code;
	qint64 showTime;
};
