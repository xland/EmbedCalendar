#pragma once

#include <Windows.h>
#include <functional>
#include "include/core/SkCanvas.h"

const uint32_t CustomMsgId{ WM_APP + 1 };
const uint32_t RefreshTimerId{ WM_APP + 2 };

enum class EventType {
    maskReady = 0,
    undoDisable,
    redoDisable,
    unselectAllTool,
    showHideSubTool
};

using MouseEventCB = std::function<void(const int&, const int&)>;
using DpiEventCB = std::function<void()>;
using CustomEventCB = std::function<void(const EventType& type, const uint32_t& msg)>;
using PaintEventCB = std::function<void(SkCanvas* canvas)>;