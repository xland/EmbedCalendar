#pragma once
#include <QColor>
class Skin
{
public:
	Skin() = default;
	~Skin() = default;
	static void init();
	static Skin* get();
	//void SetData(rapidjson::Value& data);
public:
	QColor bg;
	QColor titleBtn;
	QColor titleBtnHover;

	QColor year;
	QColor yearBtn;
	QColor yearBtnHover;
	QColor yearBtnBorder;
	QColor yearBtnToday;
	QColor yearBtnTodayHover;
	QColor yearBtnTodayBorder;

	QColor weekText;
	QColor toolTipBg;
	QColor toolTipText;
	QColor menuBg;
	QColor menuText;
	QColor menuHover;
	QColor hoverBg;
private:
	void initBlack();
	void initWhite();
};

