#pragma once

#include <QVBoxLayout>
#include <QScrollArea>

class ListContent : public QScrollArea
{
	Q_OBJECT
public:
	ListContent(QWidget *parent = nullptr);
	~ListContent();
private:
};
