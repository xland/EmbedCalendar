#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>
#include <vector>
class WeekHeader
{
public:
	WeekHeader();
	~WeekHeader();
	static void Init();
	static WeekHeader* Get();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
	void SetText(std::vector<std::string>&& param);
public:
	float bottom;
private:
	std::vector<std::string> textArr;
	std::array<float, 7> xPosArr;
	float fontSize;
	float yPos;
};