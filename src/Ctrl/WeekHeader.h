#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>
#include <rapidjson/document.h>
#include <vector>
class WeekHeader
{
public:
	WeekHeader() = default;
	~WeekHeader() = default;
	static void Init();
	static WeekHeader* Get();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void SetData(rapidjson::Value& data);
public:
	float bottom;
private:
	std::vector<std::string> textArr;
	std::array<float, 7> xPosArr;
	float fontSize;
	float yPos;
};