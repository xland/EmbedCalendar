#pragma once
#include <Windows.h> 

class EmbedHelper
{
public:
	explicit EmbedHelper(){}
	~EmbedHelper();
	static EmbedHelper* Init();
	void Embed();
	void WinResized();
	bool IsEmbed();
private:

};