#pragma once
#include <QMouseEvent>
#include <QWidget>

class ListBarBtn;
class ListBar : public QWidget
{
	Q_OBJECT
public:
	ListBar(QWidget *parent = nullptr);
	~ListBar();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	void btnEnter();
	void btnLeave();
private:
	ListBarBtn* btn;
};
