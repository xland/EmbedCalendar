#include <memory>

#include "Skin.h"

namespace {
	std::unique_ptr<Skin> skin;
}

void Skin::Init()
{
	skin = std::make_unique<Skin>();
}

Skin* Skin::Get()
{
	return skin.get();
}

void Skin::SetData(rapidjson::Value& data)
{
	auto theme = std::string{ data["backgroundTheme"].GetString() };
	auto alpha = data["backgroundOpacity"].GetFloat();
	auto alphaVal = static_cast<int>(std::round(alpha * 255));
	if (theme == "type1") {
		skin->bg = SkColorSetARGB(alphaVal, 255, 255, 255);
		skin->initWhite();
	}
	else {
		skin->bg = SkColorSetARGB(alphaVal, 0, 0, 0);
		skin->initBlack();
	}
}

void Skin::initBlack()
{
	text0 =0xFFFFFFFF;
	text1 =0xFFD2D3D4;
	text2 =0xFFA5A7A9;
	toolTipBg =0xFFFFFFFF;
	toolTipText =0xFF1F2329;
	menuBg =0xFF000000;
	menuText =0xFFFFFFFF;
	menuHover = 0x33FFFFFF;
	hoverBg = 0x3FFFFFF;
}

void Skin::initWhite()
{
	text0 = 0xFF1F2329;
	text1 = 0xFF4C4F54;
	text2 = 0xFF666666;
	toolTipBg = 0xFF1A1A1A;
	toolTipText = 0xFFFFFFFF;
	menuBg = 0xFFEDEEEE;
	menuText = 0xFF1F2329;
	menuHover = 0xFFE0E0E0;
	hoverBg = 0x30000000;
}

