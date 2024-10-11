#include <shellscalingapi.h>

#include "MainWin.h"
#include "WsConn.h"
#include "Skin.h"
#include "Font.h"
#include "Util.h"
#include "Ctrl/TitleBar.h"
#include "Ctrl/CalendarHeader.h"
#include "Ctrl/WeekHeader.h"
#include "Ctrl/CalendarBody.h"
#include "Ctrl/SwitchBtn.h"
#include "Ctrl/ListHeader.h"
#include "Ctrl/ListBody.h"
#include "Ctrl/SettingMenu.h"
#include "Ctrl/ToolTip.h"


namespace {
    std::unique_ptr<MainWin> win;
}

MainWin::MainWin(HINSTANCE _instance) :instance{_instance}
{
}



void MainWin::Init(HINSTANCE instance, std::wstring&& cmd)
{
    Util::CloseCalendarTask();
    Util::RefreshDesktop();
    Util::InitDebuger();
    win = std::make_unique<MainWin>(instance);
    Font::Init();
    Skin::Init();
    TitleBar::Init();
    CalendarHeader::Init();
    WeekHeader::Init();
    CalendarBody::Init();
    SwitchBtn::Init();
    ListHeader::Init();
    ListBody::Init();
    SettingMenu::Init();
    ToolTip::Init();
    win->getDpi();
    win->initCanvas();
    win->createWindow();
    WsConn::Init(std::move(cmd));
}
MainWin* MainWin::Get()
{
    return win.get();
}
void MainWin::Dispose()
{
    WsConn::Dispose();
    Util::RefreshDesktop();
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
    h = 580 * dpi; //todo    
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

void MainWin::ShowList()
{
    h = 860 * dpi;
    initCanvas();
    SetWindowPos(hwnd, NULL, x, y, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
}

void MainWin::HideList()
{
    h = 580 * dpi;
    initCanvas();
    SetWindowPos(hwnd, NULL, x, y, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
}


void MainWin::repaint()
{
    canvas->clear(0x00000000);
    SkPaint paint;
    paint.setColor(Skin::Get()->bg);
    paint.setAntiAlias(true);
    auto rr = SkRRect::MakeRectXY(SkRect::MakeXYWH(0, 0, w, h),4*dpi, 4*dpi);
    canvas->drawRRect(rr, paint);
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
        if (stopEventFlag) {
            break;
        }
        leftBtnDownHandlers[i](x, y);
    }
    stopEventFlag = false;
}

void MainWin::onLeftBtnUp(const int& x, const int& y)
{
    for (size_t i = 0; i < leftBtnUpHandlers.size(); i++)
    {
        if (stopEventFlag) {
            break;
        }
        leftBtnUpHandlers[i](x, y);
    }
    stopEventFlag = false;
}

void MainWin::onMouseMove(const int& x, const int& y)
{
    for (size_t i = 0; i < mouseMoveHandlers.size(); i++)
    {
        mouseMoveHandlers[i](x, y);
    }
}

void MainWin::onMouseWheel(const int& span)
{
    for (size_t i = 0; i < mouseWheelHandlers.size(); i++)
    {
        mouseWheelHandlers[i](span);
    }
}

void MainWin::onDpiChange()
{
    for (size_t i = 0; i < dpiHandlers.size(); i++)
    {
        dpiHandlers[i]();
    }
}

void MainWin::onDataReady(rapidjson::Document* d)
{
    bool hasEmbed = { false };
    int x1{ 0 }, y1{ 0 };
    try
    {
        auto data = d->operator[]("data").GetObj();
        if (data.HasMember("hasEmbed")) {
            hasEmbed = data["hasEmbed"].GetBool();
            auto pos = data["embedPosition"].GetObj();
            x1 = pos["x"].GetInt();
            y1 = pos["y"].GetInt();
        }
        Skin::Get()->SetData(data);
        TitleBar::Get()->SetData(data);
        CalendarHeader::Get()->SetData(data);
        WeekHeader::Get()->SetData(data);
        CalendarBody::Get()->SetData(data);
        ListHeader::Get()->SetData(data);
        ListBody::Get()->SetData(data);
        SettingMenu::Get()->SetData(data);
        SwitchBtn::Get()->SetData(data);
    }
    catch (const std::exception&)
    {
        std::cout << "parse receive data error" << std::endl;
        delete d;
    }
    if (dataReady) {
        Refresh();
    }
    else
    {
        dataReady = true;
        onDpiChange();
        ShowWindow(hwnd, SW_SHOW);
        SetTimer(hwnd, RefreshDataTimerId, 1000 * 60 * 60, NULL);
        if (hasEmbed) {

            auto sx = GetSystemMetrics(SM_XVIRTUALSCREEN);
            auto sy = GetSystemMetrics(SM_YVIRTUALSCREEN);
            auto sw = GetSystemMetrics(SM_CXVIRTUALSCREEN);
            auto sh = GetSystemMetrics(SM_CYVIRTUALSCREEN);
            if (x1 > sx && y1 > sy && x1 < sx + sw && y1 < sy + sh) {
                SetWindowPos(hwnd, NULL, x1, y1, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
            }
        }
    }
}
