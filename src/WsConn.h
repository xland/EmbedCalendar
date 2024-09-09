#pragma once
#include <string>
#include <thread>
#include <memory>
#include <mutex>
#include <string>
#include <curl/curl.h>

class WsConn
{
public:
	WsConn() = default;
	~WsConn() = default;
	static void Init(std::wstring&& cmdLine);
	static WsConn* Get();
	static void Dispose();
	void PostMsg(std::string&& msg);
	void connectWs();
private:
	void initWsUrl(std::wstring&& cmdLine);
	void initJson(std::string&& jsonStr);
	static size_t msgCB(char* ptr, size_t size, size_t nmemb, void* userdata);
private:
	bool dataReady{ false };
	std::unique_ptr<std::jthread> wsThread;
	CURLM* multiHandle;
	CURL* curl;
	std::string msg;
	std::string wsUrl;
};

