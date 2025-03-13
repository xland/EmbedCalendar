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
	static TitleBar* get();
	TitleBarBtn* menuBtn;
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
private:
	void menuBtnEnter();
	void menuBtnLeave();
	void pinBtnClick();
	void pinBtnEnter();
	void pinBtnLeave();
private:
	TitleBar(QWidget* parent = nullptr);
	QPoint pressPos;
	bool isDragging = false;
	TitleBarBtn* pinBtn;
	QString tipInfo;
};
