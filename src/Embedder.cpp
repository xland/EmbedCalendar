#include <format>
#include <string>
#include <include/core/SkSurface.h>
#include <include/core/SkBitmap.h>
#include <include/core/SkStream.h>
#include <include/encode/SkPngEncoder.h>
#include <Windows.h>
#include <commctrl.h>

#include "Embedder.h"
#include "MainWin.h"
#include "TypeDefine.h"
#include "Util.h"
#include "WsConn.h"

namespace {
	std::unique_ptr<Embedder> embedder;
}

Embedder::Embedder()
{
}

Embedder::~Embedder()
{
}

void Embedder::Init()
{
	embedder = std::make_unique<Embedder>();
}

Embedder* Embedder::Get()
{
	return embedder.get();
}

void Embedder::Embed()
{
    if (!hwndDefView) {
        EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
            auto self = (Embedder*)lParam;
            self->hwndDefView = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
            if (self->hwndDefView != NULL) {
                return FALSE;
            }
            return TRUE;
            }, (LPARAM)this);
    }
    auto win = MainWin::Get();
    SetWindowLongPtr(win->hwnd, GWLP_HWNDPARENT, (LONG_PTR)hwndDefView);
    auto msg = std::format(R"({{"msgName":"embedWin","data":{{"hasEmbed":true,"x":{},"y":{}}}}})", std::to_string(win->x), std::to_string(win->y));
    WsConn::Get()->PostMsg(std::move(msg));
    isEmbedded = true;
}

void Embedder::UnEmbed()
{
    auto win = MainWin::Get();
    SetWindowLongPtr(win->hwnd, GWLP_HWNDPARENT, NULL);
    WsConn::Get()->PostMsg(R"({"msgName":"embedWin","data":{"hasEmbed":false}})");
    isEmbedded = false;
}
