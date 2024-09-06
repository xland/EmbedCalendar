#include <Windows.h>
#include <dwmapi.h>
#include <versionhelpers.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include "Util.h"

void Util::InitDebuger() {
    AttachConsole(ATTACH_PARENT_PROCESS);
    AllocConsole();
    FILE* unused;
    freopen_s(&unused, "CONOUT$", "w", stdout);
    dup2(_fileno(stdout), 1);
    freopen_s(&unused, "CONOUT$", "w", stderr);
    dup2(_fileno(stdout), 2);
    std::ios::sync_with_stdio();
    std::cout << "log start:" << std::endl;
}

bool Util::IsInCircle(const SkPoint& center, const float& r, const SkPoint& mousePos)
{
    float dx = mousePos.fX - center.fX;
    float dy = mousePos.fY - center.fY;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= r;
}

std::string Util::ToStr(const wchar_t* wstr)
{
	const int count = WideCharToMultiByte(CP_UTF8, 0, wstr, wcslen(wstr), NULL, 0, NULL, NULL);
	std::string str(count, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &str[0], count, NULL, NULL);
	return str;
}

std::wstring Util::ToWStr(const char* str)
{
	int count = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstr[0], count);
	if (wstr.length() == 1 && str[0] == L'\0') {
		wstr = std::wstring{};
	}
	return wstr;
}
void Util::EnableAlpha(HWND hwnd)
{
    if (!IsWindowsVistaOrGreater()) { return; }
    BOOL is_composition_enable = false;
    DwmIsCompositionEnabled(&is_composition_enable);
    if (!is_composition_enable) { return; }
    DWORD current_color = 0;
    BOOL is_opaque = false;
    DwmGetColorizationColor(&current_color, &is_opaque);
    HRGN region = CreateRectRgn(0, 0, -1, -1);
    DWM_BLURBEHIND bb = { 0 };
    bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
    bb.hRgnBlur = region;
    bb.fEnable = TRUE;
    DwmEnableBlurBehindWindow(hwnd, &bb);
    DeleteObject(region);
}

void Util::DisableAlpha(HWND hwnd)
{
    DWM_BLURBEHIND bb = { 0 };
    bb.dwFlags = DWM_BB_ENABLE;
    bb.fEnable = FALSE;
    HRESULT hr = DwmEnableBlurBehindWindow(hwnd, &bb);
}

SkColor Util::ToColor(const std::string& colorStr) {
    auto str = colorStr.substr(1);
    unsigned int r, g, b;
    sscanf(str.data(), "%02x%02x%02x", &r, &g, &b);
    return SkColorSetARGB(255,r, g, b);
}

