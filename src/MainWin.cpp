#include <shellscalingapi.h>
#include <iostream>
#include <sstream>

#include "MainWin.h"
#include "WsConn.h"
#include "Skin.h"
#include "Font.h"
#include "Ctrl/TitleBar.h"
#include "Ctrl/CalendarHeader.h"
#include "Ctrl/WeekHeader.h"
#include "Ctrl/CalendarBody.h"

namespace {
    std::unique_ptr<MainWin> win;
}

MainWin::MainWin()
{
}

MainWin::~MainWin()
{
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, nullptr, SPIF_UPDATEINIFILE);
}

void MainWin::Init(HINSTANCE instance, std::wstring&& cmd)
{
    //std::string hwndStr = "002C089C"; 
    //std::stringstream ss;
    //ss << std::hex << hwndStr;
    //unsigned long hwndHex;
    //ss >> hwndHex; 
    //HWND hwnd = reinterpret_cast<HWND>(hwndHex);
    //PostMessage(hwnd, WM_CLOSE, 0, 0);
    
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, nullptr, SPIF_UPDATEINIFILE);
    win = std::make_unique<MainWin>();
    win->instance = instance;
    Font::Init();    
    TitleBar::Init();
    CalendarHeader::Init();
    WeekHeader::Init();
    CalendarBody::Init();
    WsConn::Init();
    win->getDpi();
    win->initCanvas();
    win->createWindow();
}
MainWin* MainWin::Get()
{
    return win.get();
}
void MainWin::Cursor(LPWSTR id)
{
    static auto _id{ IDC_NO };
    if (_id == id) {
        return;
    }
    _id = id;
    SetCursor(LoadCursor(nullptr, id));
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
    canvas->clear(Skin::Get()->bg);
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
