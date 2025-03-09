#include <memory>
#include "Skin.h"

namespace {
	std::unique_ptr<Skin> skin;
}

void Skin::init()
{
	skin = std::make_unique<Skin>();
	skin->bg = QColor(255, 255, 255,255*0.6);
	skin->initWhite();
}

Skin* Skin::get()
{
	return skin.get();
}

//void Skin::SetData(rapidjson::Value& data)
//{
//	auto theme = std::string{ data["backgroundTheme"].GetString() };
//	auto alpha = data["backgroundOpacity"].GetFloat();
//	auto alphaVal = static_cast<int>(std::round(alpha * 255));
//	if (theme == "type1") {
//		skin->bg = SkColorSetARGB(alphaVal, 255, 255, 255);
//		skin->initWhite();
//	}
//	else {
//		skin->bg = SkColorSetARGB(alphaVal, 0, 0, 0);
//		skin->initBlack();
//	}
//}

void Skin::initBlack()
{

}

void Skin::initWhite()
{
	titleBtn = QColor(100, 106, 115);
	titleBtnHover = QColor(0, 0, 0, 38);

	year = QColor(31, 35, 41);
	yearBtnHover = QColor(255, 255, 255, 76);
	yearBtnBorder = QColor(121, 123, 127);
	yearBtn = QColor(76, 79, 84);

	yearBtnTodayHover = QColor(240, 44, 56, 51);
	yearBtnTodayBorder = QColor(240, 44, 56, 255);
	yearBtnToday = QColor(240, 44, 56, 255);

	week = QColor(76, 79, 84);
	day = QColor(31, 35, 41);
	lunar = QColor(76, 79, 84);
	dayNotCurMonth = QColor(102, 102, 102);
	lunarNotCurMonth = QColor(102, 102, 102);
	dot = QColor(102, 102, 102);
	dayHover = QColor(255, 255, 255, 128);
	dayWorking = QColor(0, 122, 255);

	switchText = QColor(0, 122, 255);
}