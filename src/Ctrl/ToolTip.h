#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>

class ToolTip
{
public:
	ToolTip();
	~ToolTip();
	static void Init();
	static ToolTip* Get();
	void OnPaint(SkCanvas* canvas);
	void OnDpi();
private:

};