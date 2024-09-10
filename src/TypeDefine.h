#pragma once

#include <Windows.h>
#include <functional>
#include <iostream>

#include "include/core/SkCanvas.h"

constexpr unsigned int CustomMsgId{ WM_APP + 1 };
constexpr unsigned int RefreshTimerId{ WM_APP + 2 };
constexpr unsigned int CheckWallPaperTimerId{ WM_APP + 3 };
constexpr unsigned int DataReadyId{ WM_APP + 4 };
constexpr unsigned int RefreshDataTimerId{ WM_APP + 5 };

using MouseEventCB = std::function<void(const int&, const int&)>;
using WheelEventCB = std::function<void(const int&)>;
using DpiEventCB = std::function<void()>;
using CustomEventCB = std::function<void(const uint64_t& type, const uint64_t& msg)>;
using PaintEventCB = std::function<void(SkCanvas* canvas)>;