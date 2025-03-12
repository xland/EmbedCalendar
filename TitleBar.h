#pragma once
#include <QMouseEvent>
#include <QWidget>
#include "TitleBarBtn.h"

class TitleBar : public QWidget
{
	Q_OBJECT
public:
	~TitleBar();
	static void init();
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
private:
	void pinBtnClick();
	void enterPinBtn();
	void leavePinBtn();
private:
	TitleBar(QWidget* parent = nullptr);
	QPoint pressPos;
	bool isDragging = false;
	TitleBarBtn* pinBtn;
	TitleBarBtn* menuBtn;
	QString tipInfo;
};
