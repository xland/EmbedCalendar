#pragma once
#include <QMouseEvent>
#include <QWidget>

class ListBarBtn;
class ListBar : public QWidget
{
	Q_OBJECT
public:
	~ListBar();
	static void init();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	void btnEnter();
	void btnLeave();
	void btnClick();
private:
	ListBar(QWidget* parent = nullptr);
	QString activeDateDay;
	QString tipInfo;
	ListBarBtn* btn;
};
