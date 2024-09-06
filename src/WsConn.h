#pragma once

#define ASIO_STANDALONE

class WsConn
{
public:
	WsConn() = default;
	~WsConn() = default;
	static void Init();
	static WsConn* Get();
private:
	void initJson();
};

