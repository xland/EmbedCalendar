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

namespace {
	std::unique_ptr<WsConn> wsConn;
	std::mutex mtx;
}


size_t WsConn::msgCB(char* ptr, size_t size, size_t nmemb, void* userdata) {
	auto self = static_cast<WsConn*>(userdata);
	auto frame = curl_ws_meta(self->curl);
	auto result{ size * nmemb };
	rapidjson::Document* doc = new rapidjson::Document();
	{
		std::lock_guard<std::mutex> lock(mtx);
		self->msg.append(ptr, result);
		if (frame->bytesleft > 0)
		{
			return result;
		}
		doc->Parse(self->msg.data());
		self->msg.clear();
	}
	PostMessage(MainWin::Get()->hwnd, CustomMsgId, DataReadyId, (LPARAM)doc);
	std::wcout << L"Received message" << std::endl;
	return result;
}
void WsConn::Init(std::wstring&& cmdLine)
{
	std::wcout << L"cmdLine." << cmdLine.data() << std::endl;
	wsConn = std::make_unique<WsConn>();
	auto win = MainWin::Get();
	win->customEventHandlers.push_back(std::bind(&WsConn::OnCustomEvent, wsConn.get(), std::placeholders::_1, std::placeholders::_2));
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
	CURLcode res;
	{
		res = curl_ws_send(curl, msg.c_str(), msg.size(), &sent, 0, CURLWS_TEXT);
	}
	if (res != CURLE_OK || sent == 0) {
		std::cerr << "Failed to send WebSocket message: " << curl_easy_strerror(res) << std::endl;
	}
	else {
		std::cout << "Message sent successfully." << std::endl;
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
			//"ws://127.0.0.1:4000";
			this->wsUrl = std::format("ws://127.0.0.1:{}/{}", portStr, pathStr);
			std::cout << "ws url." << this->wsUrl << std::endl;
			break;
		}
	}
}

void WsConn::OnCustomEvent(const uint64_t& type, const uint64_t& msg)
{
#ifdef TESTDATA
	std::wstring viewData{ LR"("viewData":[{"type":"prev","year":2024,"month":6,"date":0,"startTimeStamp":1722009600000,"endTimeStamp":1722096000000,"lunarInfo":"廿二","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"prev","year":2024,"month":6,"date":28,"startTimeStamp":1722096000000,"endTimeStamp":1722182400000,"lunarInfo":"廿三","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"prev","year":2024,"month":6,"date":29,"startTimeStamp":1722182400000,"endTimeStamp":1722268800000,"lunarInfo":"廿四","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"prev","year":2024,"month":6,"date":30,"startTimeStamp":1722268800000,"endTimeStamp":1722355200000,"lunarInfo":"廿五","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"prev","year":2024,"month":6,"date":31,"startTimeStamp":1722355200000,"endTimeStamp":1722441600000,"lunarInfo":"廿六","docStatus":"","isToday":false,"isActive":false,"hasSchdule":true},
{"type":"currt","year":2024,"month":7,"date":1,"startTimeStamp":1722441600000,"endTimeStamp":1722528000000,"lunarInfo":"建军节","docStatus":"","isToday":false,"isActive":false,"hasSchdule":true},
{"type":"currt","year":2024,"month":7,"date":2,"startTimeStamp":1722528000000,"endTimeStamp":1722614400000,"lunarInfo":"廿八","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":3,"startTimeStamp":1722614400000,"endTimeStamp":1722700800000,"lunarInfo":"廿九","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":4,"startTimeStamp":1722700800000,"endTimeStamp":1722787200000,"lunarInfo":"初一","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":5,"startTimeStamp":1722787200000,"endTimeStamp":1722873600000,"lunarInfo":"初二","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":6,"startTimeStamp":1722873600000,"endTimeStamp":1722960000000,"lunarInfo":"初三","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":7,"startTimeStamp":1722960000000,"endTimeStamp":1723046400000,"lunarInfo":"立秋","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":8,"startTimeStamp":1723046400000,"endTimeStamp":1723132800000,"lunarInfo":"初五","docStatus":"","isToday":false,"isActive":false,"hasSchdule":true},
{"type":"currt","year":2024,"month":7,"date":9,"startTimeStamp":1723132800000,"endTimeStamp":1723219200000,"lunarInfo":"初六","docStatus":"","isToday":false,"isActive":false,"hasSchdule":true},
{"type":"currt","year":2024,"month":7,"date":10,"startTimeStamp":1723219200000,"endTimeStamp":1723305600000,"lunarInfo":"七夕节","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":11,"startTimeStamp":1723305600000,"endTimeStamp":1723392000000,"lunarInfo":"初八","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":12,"startTimeStamp":1723392000000,"endTimeStamp":1723478400000,"lunarInfo":"初九","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":13,"startTimeStamp":1723478400000,"endTimeStamp":1723564800000,"lunarInfo":"初十","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":14,"startTimeStamp":1723564800000,"endTimeStamp":1723651200000,"lunarInfo":"十一","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":15,"startTimeStamp":1723651200000,"endTimeStamp":1723737600000,"lunarInfo":"十二","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":16,"startTimeStamp":1723737600000,"endTimeStamp":1723824000000,"lunarInfo":"十三","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":17,"startTimeStamp":1723824000000,"endTimeStamp":1723910400000,"lunarInfo":"十四","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":18,"startTimeStamp":1723910400000,"endTimeStamp":1723996800000,"lunarInfo":"十五","docStatus":"休","isToday":false,"isActive":true,"hasSchdule":true},
{"type":"currt","year":2024,"month":7,"date":19,"startTimeStamp":1723996800000,"endTimeStamp":1724083200000,"lunarInfo":"十六","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":20,"startTimeStamp":1724083200000,"endTimeStamp":1724169600000,"lunarInfo":"十七","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":21,"startTimeStamp":1724169600000,"endTimeStamp":1724256000000,"lunarInfo":"十八","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":22,"startTimeStamp":1724256000000,"endTimeStamp":1724342400000,"lunarInfo":"处暑","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":23,"startTimeStamp":1724342400000,"endTimeStamp":1724428800000,"lunarInfo":"二十","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":24,"startTimeStamp":1724428800000,"endTimeStamp":1724515200000,"lunarInfo":"廿一","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":25,"startTimeStamp":1724515200000,"endTimeStamp":1724601600000,"lunarInfo":"廿二","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":26,"startTimeStamp":1724601600000,"endTimeStamp":1724688000000,"lunarInfo":"廿三","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":27,"startTimeStamp":1724688000000,"endTimeStamp":1724774400000,"lunarInfo":"廿四","docStatus":"班","isToday":false,"isActive":false,"hasSchdule":true},
{"type":"currt","year":2024,"month":7,"date":28,"startTimeStamp":1724774400000,"endTimeStamp":1724860800000,"lunarInfo":"廿五","docStatus":"","isToday":false,"isActive":false,"hasSchdule":true},
{"type":"currt","year":2024,"month":7,"date":29,"startTimeStamp":1724860800000,"endTimeStamp":1724947200000,"lunarInfo":"廿六","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":30,"startTimeStamp":1724947200000,"endTimeStamp":1725033600000,"lunarInfo":"廿七","docStatus":"休","isToday":true,"isActive":false,"hasSchdule":false},
{"type":"currt","year":2024,"month":7,"date":31,"startTimeStamp":1725033600000,"endTimeStamp":1725120000000,"lunarInfo":"廿八","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"next","year":2024,"month":8,"date":1,"startTimeStamp":1725120000000,"endTimeStamp":1725206400000,"lunarInfo":"廿九","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"next","year":2024,"month":8,"date":2,"startTimeStamp":1725206400000,"endTimeStamp":1725292800000,"lunarInfo":"三十","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"next","year":2024,"month":8,"date":3,"startTimeStamp":1725292800000,"endTimeStamp":1725379200000,"lunarInfo":"初一","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"next","year":2024,"month":8,"date":4,"startTimeStamp":1725379200000,"endTimeStamp":1725465600000,"lunarInfo":"初二","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"next","year":2024,"month":8,"date":5,"startTimeStamp":1725465600000,"endTimeStamp":1725552000000,"lunarInfo":"初三","docStatus":"","isToday":false,"isActive":false,"hasSchdule":false},
{"type":"next","year":2024,"month":8,"date":6,"startTimeStamp":1725552000000,"endTimeStamp":1725638400000,"lunarInfo":"初四","docStatus":"班","isToday":false,"isActive":false,"hasSchdule":false}] }})" };
	std::wstring msgData =  LR"({"msgType": "EmbedCalendar","msgName": "updateRenderData","data":{"isCn":true,"backgroundTheme":"type1","backgroundOpacity":0.7,"activeDateMonth":"2024年8月",
"activeDateDay":"周二 七月十七","weekLables":["六","日","一","二","三","四","五"],
"displayScheduleList":true,"scheduleList":[{"title":"日程标题日程标题11","desc":"日程摘要xxx11","isAllowEdit":false,"calendarColor":"#FF8866", "calendarNo":"xxxx","scheduleNo":"yyyy"},
{"title":"日程标题日程标题22","desc":"日程摘要xxx22","isAllowEdit":true,"calendarColor":"#4A53E7", "calendarNo":"xxxx22","scheduleNo":"yyyy22"}],)";
	msgData += viewData;
	msg = Util::ToStr(msgData.data());
#endif	
	if (type != DataReadyId) return;
	auto& d = *(rapidjson::Document*)msg;
	auto data = d["data"].GetObj();
	{
		auto theme = data["backgroundTheme"].GetString();
		auto alpha = data["backgroundOpacity"].GetFloat();
		Skin::Init(theme, alpha);
	}
	{
		auto header = data["activeDateMonth"].GetString();
		CalendarHeader::Get()->SetText(std::move(header));
	}
	{
		auto arr = data["weekLables"].GetArray();
		std::vector<std::string> param;
		for (size_t i = 0; i < arr.Size(); i++)
		{
			param.push_back(arr[i].GetString());
		}
		WeekHeader::Get()->SetText(std::move(param));
	}
	{
		auto arr = data["viewData"].GetArray();
		std::string currtStr = Util::ToStr(L"currt");
		std::vector<DateItem> param;
		for (auto& data:arr)
		{
			DateItem item;
			item.date = std::to_string(data["date"].GetUint());
			item.year = std::to_string(data["year"].GetUint());
			item.month = std::to_string(data["month"].GetUint());
			//auto str = Util::ToWStr(jsonStr.data());
			item.hasSchdule = data.HasMember("hasSchdule")?data["hasSchdule"].GetBool():false;
			item.isActive = data["isActive"].GetBool();
			item.isToday = data["isToday"].GetBool();
			item.lunar = data["lunarInfo"].GetString();
			item.isCurrt = data["type"].GetString() == currtStr;
			item.badge = data["docStatus"].GetString();
			param.push_back(std::move(item));
		}
		CalendarBody::Get()->SetText(std::move(param));
	}
	{
		ListHeader::Get()->text = data["activeDateDay"].GetString();
	}
	{
		auto arr = data["scheduleList"].GetArray();
		std::vector<ListItem> param;
		for (auto& data : arr)
		{
			ListItem item;
			item.title = data["title"].GetString();
			item.desc = data["desc"].GetString();
			item.isAllowEdit = data["isAllowEdit"].GetBool();
			item.color = Util::ToColor(data["calendarColor"].GetString());
			item.calendarNo = data["calendarNo"].GetString();
			item.scheduleNo = data["scheduleNo"].GetString();
			param.push_back(std::move(item));
		}
		ListBody::Get()->SetText(std::move(param));
	}
	{
		auto lang = data["lang"].GetObj();
		auto settingMenu = SettingMenu::Get();
		settingMenu->menuText[0] = std::string{ lang["setting"].GetString() };
		settingMenu->menuText[1] = std::string{lang["help"].GetString() };
		settingMenu->menuText[2] = std::string{lang["advise"].GetString() };
		settingMenu->menuText[3] = std::string{lang["signout"].GetString() };
	}
	delete (rapidjson::Document*)msg;
	if (dataReady) {
		MainWin::Get()->Refresh();
	}
	else
	{
		MainWin::Get()->OnWsDataReady();
		dataReady = true;
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
			}
		});
}
