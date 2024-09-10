#include <Windows.h>
#include <format>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>


#include "TypeDefine.h"
#include "Util.h"
#include "WsConn.h"
#include "MainWin.h"
#include "Skin.h"
#include "DateItem.h"
#include "ListItem.h"
#include "Ctrl/CalendarHeader.h"
#include "Ctrl/WeekHeader.h"
#include "Ctrl/CalendarBody.h"
#include "Ctrl/ListHeader.h"
#include "Ctrl/ListBody.h"
#include "Ctrl/SettingMenu.h"
#include "Ctrl/SwitchBtn.h"

namespace {
	std::unique_ptr<WsConn> wsConn;
	std::mutex mtx;
}
size_t WsConn::msgCB(char* ptr, size_t size, size_t nmemb, void* userdata) {
	auto self = static_cast<WsConn*>(userdata);
	auto frame = curl_ws_meta(self->curl);
	auto result{ size * nmemb };
	rapidjson::Document* doc{nullptr};
	{
		std::lock_guard<std::mutex> lock(mtx);
		self->msg.append(ptr, result);
		if (frame->bytesleft > 0)
		{
			return result;
		}
		doc = new rapidjson::Document();
		doc->Parse(self->msg.data());
		self->msg.clear();
	}
	PostMessage(MainWin::Get()->hwnd, DataReadyId, 0, (LPARAM)doc);
	std::cout << "received one message" << std::endl;
	return result;
}
void WsConn::Init(std::wstring&& cmdLine)
{
	wsConn = std::make_unique<WsConn>();
	wsConn->initWsUrl(std::move(cmdLine));
	wsConn->connectWs();
}

WsConn* WsConn::Get()
{
	return wsConn.get();
}

void WsConn::Dispose()
{
	auto self = wsConn.get();
	self->wsThread.reset();
	curl_multi_remove_handle(self->multiHandle, self->curl);
	curl_easy_cleanup(self->curl);
	curl_multi_cleanup(self->multiHandle);
	curl_global_cleanup();
	wsConn.reset();
}

void WsConn::PostMsg(std::string&& msg)
{
	msg.insert(1, R"("msgType":"EmbedCalendar",)");
	size_t sent{ 0 };
	CURLcode res = curl_ws_send(curl, msg.c_str(), msg.size(), &sent, 0, CURLWS_TEXT);
	if (res != CURLE_OK || sent == 0) {
		std::cerr << "Failed to send ws message: " << curl_easy_strerror(res) << std::endl;
	}
}

void WsConn::initWsUrl(std::wstring&& cmdLine)
{
	std::wistringstream stream(cmdLine);
	std::wstring arg;
	while (stream >> std::quoted(arg)) {
		auto pos = arg.find(L"_");
		if (pos == std::wstring::npos) {
			continue;
		}
		else
		{
			auto portStr = Util::ToStr(arg.substr(pos + 1).data());
			auto pathStr = Util::ToStr(arg.substr(0, pos).data());
			this->wsUrl = std::format("ws://127.0.0.1:{}/{}", portStr, pathStr);
			std::cout << "ws url:" << this->wsUrl << std::endl;
			break;
		}
	}
}

void WsConn::connectWs()
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
	multiHandle = curl_multi_init();
	curl = curl_easy_init();
	if (!curl) {
		std::cerr << "Failed to initialize CURL." << std::endl;
		return;
	}
	curl_easy_setopt(curl, CURLOPT_URL, wsUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WsConn::msgCB);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

	struct curl_ws_frame ws_frame = { 0 };
	ws_frame.flags = CURLWS_TEXT;
	curl_easy_setopt(curl, CURLOPT_WS_OPTIONS, &ws_frame);
	curl_multi_add_handle(multiHandle, curl);

	wsThread = std::make_unique<std::jthread>([this](std::stop_token token) {
			int still_running{1};
			CURLMcode mc{ CURLM_OK };
			while (still_running && mc == CURLM_OK && !token.stop_requested())
			{
				mc = curl_multi_perform(multiHandle, &still_running);
				if (still_running) {
					curl_multi_poll(multiHandle, nullptr, 0, 100, nullptr); // 等待事件
				}
				else {
					PostMessage(MainWin::Get()->hwnd, WM_CLOSE, 0, 0);
				}
			}
		});
}
