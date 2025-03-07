#pragma once
#include <QMouseEvent>
#include <QWidget>
#include <QColor>

class YearBarBtnBase : public QWidget
{
	Q_OBJECT
public:
	YearBarBtnBase(QWidget *parent = nullptr);
	~YearBarBtnBase();
protected:
	virtual void paintEvent(QPaintEvent* event) override;
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
protected:
	QColor hoverColor;
	QColor borderColor;
	QColor color;
private:
	bool isHover{false};
};
