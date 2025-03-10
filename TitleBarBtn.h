#pragma once
#include <QMouseEvent>
#include <QWidget>

class TitleBarBtn : public QWidget
{
	Q_OBJECT
public:
	TitleBarBtn(const uint& code, QWidget *parent = nullptr);
	~TitleBarBtn();
signals:
	void click();
protected:
	void paintEvent(QPaintEvent* event) override;
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
private:
	bool isHover{ false };
	uint code;
};
