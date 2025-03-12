#pragma once

#include <QVBoxLayout>
#include <QScrollArea>

class ListContent : public QScrollArea
{
	Q_OBJECT
public:
	~ListContent();
	void scroll(const int& dis);
	static void init();
	static ListContent* get();
private:
	ListContent(QWidget* parent = nullptr);
	void updateData(const QJsonObject& obj);
	QVBoxLayout* layout;
};
