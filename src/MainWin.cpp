#include "MainWin.h"

MainWin::MainWin()
{
}

MainWin::~MainWin()
{
}

void MainWin::Init()
{
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

void MainWin::onCustomMsg(const EventType& type, const uint32_t& msg)
{
    for (size_t i = 0; i < customEventHandlers.size(); i++)
    {
        customEventHandlers[i](type, msg);
    }
}
