#include "ListBody.h"
#include "../Font.h"
#include "../MainWin.h"
#include "../TypeDefine.h"
#include "../Skin.h"
#include "ListHeader.h"
#include "SwitchBtn.h"
#include "../Util.h"
#include "../Embedder.h"

namespace {
	std::unique_ptr<ListBody> listBody;
}

ListBody::ListBody()
{
}

ListBody::~ListBody()
{
}

void ListBody::Init()
{
	listBody = std::make_unique<ListBody>();
	auto win = MainWin::Get();
	win->paintHandlers.push_back(std::bind(&ListBody::OnPaint, listBody.get(), std::placeholders::_1));
	win->dpiHandlers.push_back(std::bind(&ListBody::OnDpi, listBody.get()));
	win->mouseMoveHandlers.push_back(std::bind(&ListBody::OnMouseMove, listBody.get(), std::placeholders::_1, std::placeholders::_2));
	win->leftBtnDownHandlers.push_back(std::bind(&ListBody::OnLeftBtnDown, listBody.get(), std::placeholders::_1, std::placeholders::_2));
}

ListBody* ListBody::Get()
{
	return listBody.get();
}

void ListBody::OnPaint(SkCanvas* canvas)
{
	if (!SwitchBtn::Get()->listVisible) return;
	auto fontText = Font::GetText();
	fontText->setSize(emptyFontSize);
	SkPaint paint;
	paint.setColor(Skin::Get()->text1);
	canvas->drawString(emptyText.data(), emptyX, emptyY, *fontText, paint);
}

void ListBody::OnDpi()
{
	auto win = MainWin::Get();
	emptyFontSize = 18 * win->dpi;
	auto font = Font::GetText();
	SkRect measureRect;
	font->setSize(emptyFontSize);
	emptyText = Util::ToStr(L"暂无日程");
	font->measureText(emptyText.data(), emptyText.size(), SkTextEncoding::kUTF8, &measureRect);
	emptyX = win->w / 2 - measureRect.width() / 2 - measureRect.fLeft;
	emptyY = 680*win->dpi;
}

void ListBody::OnLeftBtnDown(const int& x, const int& y)
{
}

void ListBody::OnMouseMove(const int& x, const int& y)
{
}
