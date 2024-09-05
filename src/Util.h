#pragma once
#include <string>

class Util
{
public:
	Util();
	~Util();
	static std::string ToStr(const wchar_t* wstr);
	static std::wstring ToWStr(const char* str);
	static void EnableAlpha(HWND hwnd);
	static void DisableAlpha(HWND hwnd);
	static void InitDebuger();
private:

};