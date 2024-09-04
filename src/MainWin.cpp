#include <shellscalingapi.h>
#include <iostream>
#include <sstream>

#include "MainWin.h"
#include "./Ctrl/TitleBar.h"
#include "./Ctrl/CalendarHeader.h"
#include "WsConn.h"
#include "Skin.h"

MainWin::MainWin()
{
}

MainWin::~MainWin()
{
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, nullptr, SPIF_UPDATEINIFILE);
}

void MainWin::Init()
{
    //std::string hwndStr = "002C089C"; 
    //std::stringstream ss;
    //ss << std::hex << hwndStr;
    //unsigned long hwndHex;
    //ss >> hwndHex; 
    //HWND hwnd = reinterpret_cast<HWND>(hwndHex);
    //PostMessage(hwnd, WM_CLOSE, 0, 0);


    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, nullptr, SPIF_UPDATEINIFILE);
    titleBar = std::make_unique<TitleBar>();
    titleBar->Init();
    calendarHeader = std::make_unique<CalendarHeader>();
    calendarHeader->Init();
    skin = std::make_unique<Skin>();
    wsConn = std::make_unique<WsConn>();
    wsConn->Init();
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
    w = 580 * dpi;
    h = 580 * dpi;
    onDpiChange();
}

void MainWin::initCanvas()
{
    auto dataSize = h * w;
    winPix.resize(dataSize);
    SkImageInfo info = SkImageInfo::MakeN32Premul(w, h);
    auto rowSize = w * sizeof(SkColor);
    canvas = SkCanvas::MakeRasterDirect(info, &winPix.front(), rowSize);
}
void MainWin::Refresh()
{
    if (refreshFlag) {
        return;
    }
    refreshFlag = SetTimer(hwnd, RefreshTimerId, 15, NULL);
}

void MainWin::Close()
{
    CloseWindow(hwnd);
    DestroyWindow(hwnd);
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, nullptr, SPIF_UPDATEINIFILE);
    PostQuitMessage(0);
}

void MainWin::repaint()
{
    canvas->clear(skin->bg);
    for (size_t i = 0; i < paintHandlers.size(); i++)
    {
        paintHandlers[i](canvas.get());
    }
    PAINTSTRUCT ps;
    auto dc = BeginPaint(hwnd, &ps);
    BITMAPINFO bmpInfo = { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB, h * 4 * w, 0, 0, 0, 0 };
    StretchDIBits(dc, 0, 0, w, h, 0, 0, w, h, &winPix.front(), &bmpInfo, DIB_RGB_COLORS, SRCCOPY);
    ReleaseDC(hwnd, dc);
    EndPaint(hwnd, &ps);
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
