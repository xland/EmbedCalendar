#pragma once
#include <string>
#include <thread>
#include <memory>
#include <mutex>
#include <string>
#include <curl/curl.h>
#include <rapidjson/document.h>

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
	void OnCustomEvent(const uint32_t& type, const uint32_t& msg);
private:
	void initWsUrl(std::wstring&& cmdLine);
	static size_t msgCB(char* ptr, size_t size, size_t nmemb, void* userdata);
private:
	bool dataReady{ false };
	rapidjson::Document d;
	std::unique_ptr<std::jthread> wsThread;
	CURLM* multiHandle;
	CURL* curl;
	std::string msg;
	std::string wsUrl;
};

