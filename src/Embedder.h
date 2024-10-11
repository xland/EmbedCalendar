#pragma once
#include <include/core/SkColor.h>
#include <Windows.h>

class Embedder
{
public:
	Embedder();
	~Embedder();
	static void Init();
	static Embedder* Get();
	void Embed();
	void UnEmbed();
public:
	bool isEmbedded{false};
private:
private:
	HWND hwndDefView{ nullptr };
};

