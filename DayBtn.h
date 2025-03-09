#pragma once
#include <QMouseEvent>
#include <QWidget>

class DayBtn : public QWidget
{
	Q_OBJECT
public:
	DayBtn(const int& index,QWidget *parent = nullptr);
	~DayBtn();
protected:
	void paintEvent(QPaintEvent* event) override;
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
private:
	bool isHover{ false };
	int index;
};
