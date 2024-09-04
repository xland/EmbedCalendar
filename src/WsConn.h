#pragma once
class WsConn
{
public:
	WsConn();
	~WsConn();
	static void Init();
	static WsConn* Get();
private:
	void initJson();
};

