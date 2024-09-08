#pragma once
#include <thread>
#include <memory>

class WsConn
{
public:
	WsConn() = default;
	~WsConn() = default;
	static void Init();
	static WsConn* Get();
	static void Dispose();
private:
	void initJson();
	std::unique_ptr<std::jthread> wsThread;
};

