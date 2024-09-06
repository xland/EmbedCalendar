#include "ListHeader.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../TypeDefine.h"
#include "../Skin.h"
#include "CalendarBody.h"
#include "../Util.h"
#include "../Embedder.h"
#include "SwitchBtn.h"

namespace {
	std::unique_ptr<ListHeader> listHeader;
}

ListHeader::ListHeader()
{
}

ListHeader::~ListHeader()
{
}

void ListHeader::Init()
{
	listHeader = std::make_unique<ListHeader>();
	auto win = MainWin::Get();
	win->paintHandlers.push_back(std::bind(&ListHeader::OnPaint, listHeader.get(), std::placeholders::_1));
	win->dpiHandlers.push_back(std::bind(&ListHeader::OnDpi, listHeader.get()));
	win->mouseMoveHandlers.push_back(std::bind(&ListHeader::OnMouseMove, listHeader.get(), std::placeholders::_1, std::placeholders::_2));
	win->leftBtnDownHandlers.push_back(std::bind(&ListHeader::OnLeftBtnDown, listHeader.get(), std::placeholders::_1, std::placeholders::_2));
}

ListHeader* ListHeader::Get()
{
	return listHeader.get();
}

void ListHeader::OnPaint(SkCanvas* canvas)
{
	if (!SwitchBtn::Get()->listVisible) return;
	auto fontText = Font::GetText();
	auto fontIcon = Font::GetIcon();
	fontText->setSize(fontSize);
	fontIcon->setSize(fontSize);
	SkPaint paint;
	paint.setColor(0xFF007AFF);
	canvas->drawString(text.data(), textX, textY, *fontText, paint);
	const char* iconCode{ (const char*)u8"\ue70b" };
	canvas->drawString(iconCode, iconX, textY, *fontIcon, paint);
}

void ListHeader::OnDpi()
{
	auto win = MainWin::Get();
	fontSize = 24 * win->dpi;
	textX = 36 * win->dpi;
	textY = CalendarBody::Get()->b + 56 * win->dpi;
	iconX = win->w - 62* win->dpi;
	btnSize = 40 * win->dpi;
	btnX = iconX - 10 * win->dpi;
	btnY = textY - 30 *win->dpi;
}

void ListHeader::OnLeftBtnDown(const int& x, const int& y)
{
	if (!SwitchBtn::Get()->listVisible) return;
	if (!isMouseIn) return;
}

void ListHeader::OnMouseMove(const int& x, const int& y)
{
	if (!SwitchBtn::Get()->listVisible) return;
	auto flag = (x>btnX && x<btnX+btnSize && y>btnY && y<btnY+btnSize);
	auto win = MainWin::Get();
	if (!flag && isMouseIn) {
		isMouseIn = false;
		MainWin::Cursor(IDC_ARROW);
		return;
	}
	if (flag && !isMouseIn) {
		isMouseIn = true;
		MainWin::Cursor(IDC_HAND);
	}
}
