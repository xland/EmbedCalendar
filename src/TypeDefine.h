#pragma once

#include <Windows.h>
#include <functional>
#include "include/core/SkCanvas.h"

enum class EventType {
    maskReady = 0,
    undoDisable,
    redoDisable,
    unselectAllTool,
    showHideSubTool
};

using MouseEventCB = std::function<void(const int&, const int&)>;
using CustomEventCB = std::function<void(const EventType& type, const uint32_t& msg)>;
using PaintEventCB = std::function<void(SkCanvas* canvas)>;