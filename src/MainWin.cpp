#include <shellscalingapi.h>

#include "MainWin.h"
#include "./Ctrl/TitleBar.h"

MainWin::MainWin()
{
}

MainWin::~MainWin()
{
}

void MainWin::Init()
{
    titleBar = std::make_unique<TitleBar>();
    titleBar->Init();
    getDpi();
    initCanvas();
    createWindow();
}
void MainWin::getDpi()
{
    UINT dpiX = 0, dpiY = 0;
    HMONITOR hMonitor = MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY);
    GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
    dpi = dpiX / 96.0f;
    w = w * dpi;
    h = h * dpi;
    onDpiChange();
}

void MainWin::initCanvas()
{
    auto dataSize = h * w;
    winPix.resize(dataSize, 0x00000000);
    SkImageInfo info = SkImageInfo::MakeN32Premul(w, h);
    auto rowSize = w * sizeof(SkColor);
    canvas = SkCanvas::MakeRasterDirect(info, &winPix.front(), rowSize);
}
void MainWin::Refresh()
{
    if (refreshFlag) {
        return;
    }
    refreshFlag = true;
    SetTimer(hwnd, RefreshTimerId, 15, NULL);
}

void MainWin::repaint()
{
    canvas->clear(0x88000000);
    for (size_t i = 0; i < paintHandlers.size(); i++)
    {
        paintHandlers[i](canvas.get());
    }
    HDC hdc = GetDC(hwnd);
    HDC hCompatibleDC = CreateCompatibleDC(hdc);
    HBITMAP bitmap = CreateCompatibleBitmap(hdc, w, h);
    DeleteObject(SelectObject(hCompatibleDC, bitmap));
    BITMAPINFO info = { sizeof(BITMAPINFOHEADER), w, 0 - h, 1, 32, BI_RGB, w * 4 * h, 0, 0, 0, 0 };
    SetDIBits(hdc, bitmap, 0, h, &winPix.front(), &info, DIB_RGB_COLORS);
    BLENDFUNCTION blend = { .BlendOp{AC_SRC_OVER}, .SourceConstantAlpha{255}, .AlphaFormat{AC_SRC_ALPHA} };
    POINT pSrc = { 0, 0 };
    SIZE sizeWnd = { w, h };
    UpdateLayeredWindow(hwnd, hdc, NULL, &sizeWnd, hCompatibleDC, &pSrc, NULL, &blend, ULW_ALPHA);
    DeleteObject(bitmap);
    DeleteDC(hCompatibleDC);
    ReleaseDC(hwnd, hdc);
}

void MainWin::onLeftBtnDown(const int& x, const int& y)
{
    for (size_t i = 0; i < leftBtnDownHandlers.size(); i++)
    {
        leftBtnDownHandlers[i](x, y);
    }
}

void MainWin::onLeftBtnUp(const int& x, const int& y)
{
    for (size_t i = 0; i < leftBtnUpHandlers.size(); i++)
    {
        leftBtnUpHandlers[i](x, y);
    }
}

void MainWin::onMouseMove(const int& x, const int& y)
{
    for (size_t i = 0; i < mouseMoveHandlers.size(); i++)
    {
        mouseMoveHandlers[i](x, y);
    }
}

void MainWin::onMouseDrag(const int& x, const int& y)
{
    for (size_t i = 0; i < mouseDragHandlers.size(); i++)
    {
        mouseDragHandlers[i](x, y);
    }
}

void MainWin::onDpiChange()
{
    for (size_t i = 0; i < dpiHandlers.size(); i++)
    {
        dpiHandlers[i]();
    }
}

void MainWin::onCustomMsg(const EventType& type, const uint32_t& msg)
{
    for (size_t i = 0; i < customEventHandlers.size(); i++)
    {
        customEventHandlers[i](type, msg);
    }
}
