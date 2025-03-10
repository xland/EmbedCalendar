#pragma once
#include <QMouseEvent>
#include <QWidget>
#include "TitleBarBtn.h"

class TitleBar : public QWidget
{
	Q_OBJECT
public:
	TitleBar(QWidget *parent = nullptr);
	~TitleBar();
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
private:
	void pinBtnClick();
private:
	QPoint pressPos;
	bool isDragging = false;
	TitleBarBtn* pinBtn;
	TitleBarBtn* menuBtn;
};
