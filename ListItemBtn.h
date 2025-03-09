#pragma once
#include <QMouseEvent>
#include <QWidget>

class ListItemBtn : public QWidget
{
	Q_OBJECT
public:
	ListItemBtn(const uint& code, QWidget *parent = nullptr);
	~ListItemBtn();
protected:
	void paintEvent(QPaintEvent* event) override;
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
private:
	bool isHover{ false };
	uint code;
};
