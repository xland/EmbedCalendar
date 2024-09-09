#pragma once

#include <Windows.h>
#include <functional>
#include <iostream>

#include "include/core/SkCanvas.h"

constexpr unsigned int CustomMsgId{ WM_APP + 1 };
constexpr unsigned int RefreshTimerId{ WM_APP + 2 };
constexpr unsigned int CheckWallPaperTimerId{ WM_APP + 3 };
constexpr unsigned int DataReadyId{ WM_APP + 4 };

using MouseEventCB = std::function<void(const int&, const int&)>;
using DpiEventCB = std::function<void()>;
using CustomEventCB = std::function<void(const uint32_t& type, const uint32_t& msg)>;
using PaintEventCB = std::function<void(SkCanvas* canvas)>;