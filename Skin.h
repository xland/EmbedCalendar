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

	QColor week;
	QColor day;
	QColor dayHover;
	QColor lunar;
	QColor dayNotCurMonth;
	QColor lunarNotCurMonth;
	QColor dot;
	QColor dayWorking;

	QColor switchText;

	QColor listItemText1;
	QColor listItemText2;
	QColor listItemBtn;

	QColor tipInfo;
	QColor tipInfoBg;

private:
	void initBlack();
	void initWhite();
};

