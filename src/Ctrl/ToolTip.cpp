#include "ToolTip.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../TypeDefine.h"
#include "../Skin.h"
#include "../Util.h"

namespace {
	std::unique_ptr<ToolTip> toolTip;
}

ToolTip::ToolTip()
{
}

ToolTip::~ToolTip()
{
}

void ToolTip::Init()
{
	toolTip = std::make_unique<ToolTip>();
	auto win = MainWin::Get();
	win->paintHandlers.push_back(std::bind(&ToolTip::OnPaint, toolTip.get(), std::placeholders::_1));
	win->dpiHandlers.push_back(std::bind(&ToolTip::OnDpi, toolTip.get()));
}

ToolTip* ToolTip::Get()
{
	return toolTip.get();
}

void ToolTip::OnPaint(SkCanvas* canvas)
{
}

void ToolTip::OnDpi()
{
}
