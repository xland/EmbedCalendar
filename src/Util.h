#pragma once
#include <string>
#include <include/core/SkPoint.h>
#include <include/core/SkColor.h>

class Util
{
public:
	Util() = default;
	~Util() = default;
	static std::string ToStr(const wchar_t* wstr);
	static std::wstring ToWStr(const char* str);
	static void EnableAlpha(HWND hwnd);
	static void DisableAlpha(HWND hwnd);
	static void InitDebuger();
	static bool IsInCircle(const SkPoint& center, const float& r, const SkPoint& mousePos);
	static SkColor ToColor(const std::string& colorStr);
private:

};