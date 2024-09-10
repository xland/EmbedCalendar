#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>
#include <rapidjson/document.h>

class ListHeader
{
public:
	ListHeader() = default;
	~ListHeader() = default;
	static void Init();
	static ListHeader* Get();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void OnLeftBtnDown(const int& x, const int& y);
	void OnMouseMove(const int& x, const int& y);
	void SetData(rapidjson::Value& data);
public:
	float btnX, btnY, btnSize;
private:
	std::string text;
	float fontSize;
	float textX, textY, iconX;
	bool isMouseIn{ false };
};

