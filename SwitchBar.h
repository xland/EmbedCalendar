#pragma once
#include <QMouseEvent>
#include <QWidget>

class SwitchBar : public QWidget
{
	Q_OBJECT
public:
	SwitchBar(QWidget *parent = nullptr);
	~SwitchBar();
protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
private:
};
