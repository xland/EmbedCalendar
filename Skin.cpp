#include <QCoreApplication>
#include <QJsonObject>
#include "Skin.h"
#include "WsConn.h"

namespace {
	Skin* skin;
}

Skin::Skin(QObject* parent) : QObject(parent)
{
	connect(WsConn::get(), &WsConn::onData, this, &Skin::setData);
}

Skin::~Skin()
{
}

void Skin::init()
{
	skin = new Skin(qApp);
}

Skin* Skin::get()
{
	return skin;
}

void Skin::setData(const QJsonObject& data)
{
	auto theme = data["backgroundTheme"].toString();
	auto alpha = data["backgroundOpacity"].toDouble();
	auto alphaVal = static_cast<int>(std::round(alpha * 255));
	if (theme == "type1") {
		skin->bg = QColor(255, 255, 255, alphaVal);
		skin->initWhite();
	}
	else {
		skin->bg = QColor(0, 0, 0, alphaVal);
		skin->initBlack();
	}
}

void Skin::initBlack()
{

}

void Skin::initWhite()
{
	bg = QColor(255, 255, 255, 153);
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
	dayActive = QColor(240, 44, 56);
	dayWorking = QColor(0, 122, 255);

	switchText = QColor(0, 122, 255);

	listItemText1 = QColor(31, 35, 41);
	listItemText2 = QColor(102, 102, 102);
	listItemBg = QColor(255, 255, 255,51);
	listItemBtn = QColor(76, 79, 84);

	tipInfoBg = QColor(26, 26, 26);
	tipInfo = QColor(255, 255, 255);
}