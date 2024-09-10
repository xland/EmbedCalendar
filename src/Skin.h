#pragma once
#include <include/core/SkColor.h>
#include <string>
#include <rapidjson/document.h>

class Skin
{
public:
	Skin() = default;
	~Skin() = default;
	static void Init();
	static Skin* Get();
	void SetData(rapidjson::Value& data);
public:
	SkColor bg;
	SkColor text0;
	SkColor text1;
	SkColor text2;
	SkColor toolTipBg;
	SkColor toolTipText;
	SkColor menuBg;
	SkColor menuText;
	SkColor menuHover;
	SkColor hoverBg;
private:
	void initBlack();
	void initWhite();
};

