#pragma once
#include <include/core/SkColor.h>
#include <string>

class Skin
{
public:
	Skin();
	~Skin();
	static void Init(const std::string& theme,const float& alpha);
	static Skin* Get();
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

