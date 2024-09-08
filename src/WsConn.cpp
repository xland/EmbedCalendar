#include <string>
#include <Windows.h>
#include <format>
#include <rapidjson/document.h>
#include <iostream>
#include <string>
#include <vector>
#include <libwebsockets.h>

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

namespace {
	std::unique_ptr<WsConn> wsConn;
	bool wsStopFlag{ false };
	static struct my_conn {
		lws_sorted_usec_list_t	sul; /* schedule connection retry */
		struct lws* wsi; /* related wsi if any */
		uint16_t retry_count; /* count of consequetive retries */
	} mco;
	static struct lws_context* context;
	static const uint32_t backoffMs[] = { 1000, 2000, 3000, 4000, 5000 };
	static const lws_retry_bo_t retry = {
		.retry_ms_table = backoffMs,
		.retry_ms_table_count = LWS_ARRAY_SIZE(backoffMs),
		.conceal_count = LWS_ARRAY_SIZE(backoffMs),
		.secs_since_valid_ping = 3,  /* force PINGs after secs idle */
		.secs_since_valid_hangup = 10, /* hangup after secs idle */
		.jitter_percent = 20,
	};
}


static void startConnect(lws_sorted_usec_list_t* sul)
{
	struct my_conn* m = lws_container_of(sul, struct my_conn, sul);
	struct lws_client_connect_info i;
	memset(&i, 0, sizeof(i));
	i.context = context;
	i.port = 8800;
	i.address = "124.222.224.186";
	i.path = "/";
	i.host = i.address;
	i.origin = i.address;
	i.protocol = "dumb-increment-protocol";
	i.local_protocol_name = "lws-minimal-client";
	i.pwsi = &m->wsi;
	i.retry_and_idle_policy = &retry;
	i.userdata = m;
	if (!lws_client_connect_via_info(&i))		
		//Failed... schedule a retry... we can't use the _retry_wsi() convenience wrapper api here because no valid wsi at this point.		
		if (lws_retry_sul_schedule(context, 0, sul, &retry, startConnect, &m->retry_count)) {
			lwsl_err("%s: connection attempts exhausted\n", __func__);
			wsStopFlag = true;
		}
}

static int wsCB(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len)
{
	struct my_conn* m = (struct my_conn*)user;
	switch (reason) {
		case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		{
			lwsl_err("CLIENT_CONNECTION_ERROR: %s\n", in ? (char*)in : "(null)");
			if (lws_retry_sul_schedule_retry_wsi(wsi, &m->sul, startConnect, &m->retry_count)) {
				lwsl_err("%s: connection attempts exhausted\n", __func__);
				wsStopFlag = true;
			}
			return 0;
		}
		case LWS_CALLBACK_CLIENT_RECEIVE: {
			//((char*)in)[len] = '\0';
			//std::string msgStr((char*)in, len);
			auto wmsgStr = Util::ToWStr((char*)in);
			lwsl_hexdump_notice(in, len);
			break;
		}
		case LWS_CALLBACK_CLIENT_ESTABLISHED: //链接建立
		{
			lwsl_user("%s: established\n", __func__);
			break;
		}
		case LWS_CALLBACK_CLIENT_WRITEABLE:
		{
			std::string msgStr;
			break;
			//msgStr.insert(0, LWS_PRE, ' ');
			//auto sendSize = lws_write(wsi, (unsigned char*)msgStr.c_str() + LWS_PRE, msgStr.length() - LWS_PRE, LWS_WRITE_TEXT);
			//if (sendSize < 0) {
			//	lwsl_err("Error sending\n");
			//	return -1;
			//}
		}
		case LWS_CALLBACK_CLIENT_CLOSED:
		{
			lwsl_err("CLIENT_CONNECTION_ERROR: %s\n", in ? (char*)in : "(null)");
			if (lws_retry_sul_schedule_retry_wsi(wsi, &m->sul, startConnect, &m->retry_count)) {
				lwsl_err("%s: connection attempts exhausted\n", __func__);
				wsStopFlag = true;
			}
			return 0;
		}
		default: {
			break;
		}			
	}
	return lws_callback_http_dummy(wsi, reason, user, in, len);
}

static const struct lws_protocols protocols[] = {
	{ "embedCalendar", wsCB, 0, 0, 0, NULL, 0 },
	LWS_PROTOCOL_LIST_TERM
};


void initWs(std::stop_token token) {
	struct lws_context_creation_info info;
	memset(&info, 0, sizeof(info));
	lwsl_user("LWS minimal ws client\n");
	info.port = CONTEXT_PORT_NO_LISTEN; /* we do not run any server */
	info.protocols = protocols;
	info.fd_limit_per_thread = 1 + 1 + 1;
	context = lws_create_context(&info);
	lws_sul_schedule(context, 0, &mco.sul, startConnect, 6);
	int n = 0;
	while (n >= 0 && !wsStopFlag && !token.stop_requested()) { // 
		n = lws_service(context, 0);
	}		
	lws_context_destroy(context);
	lwsl_user("Completed\n");


	//using namespace std::literals::chrono_literals;
	//static int value{ 1 };
	//while (!token.stop_requested())
	//{
	//	std::cout << value++ << std::endl << std::flush;
	//	std::this_thread::sleep_for(200ms);
	//}
	//std::cout << "thread stop" << std::endl;
}

void WsConn::Init()
{
	wsConn = std::make_unique<WsConn>();
	wsConn->wsThread = std::make_unique<std::jthread>(initWs);
	wsConn->initJson();
}

WsConn* WsConn::Get()
{
	return wsConn.get();
}

void WsConn::Dispose()
{
	wsConn.reset();
}

void WsConn::initJson()
{
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
	std::wstring jsonStr{ LR"({"msgType": "EmbedCalendar","msgName": "updateRenderData","data":{"isCn":true,"backgroundTheme":"type1","backgroundOpacity":0.7,"activeDateMonth":"2024年8月",
"activeDateDay":"周二 七月十七","weekLables":["六","日","一","二","三","四","五"],
"displayScheduleList":true,"scheduleList":[{"title":"日程标题日程标题11","desc":"日程摘要xxx11","isAllowEdit":false,"calendarColor":"#FF8866", "calendarNo":"xxxx","scheduleNo":"yyyy"},
{"title":"日程标题日程标题22","desc":"日程摘要xxx22","isAllowEdit":true,"calendarColor":"#4A53E7", "calendarNo":"xxxx22","scheduleNo":"yyyy22"}],)" };
	jsonStr += viewData;
	auto str = Util::ToStr(jsonStr.data());
	rapidjson::Document d;
	d.Parse(str.data());
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
			item.hasSchdule = data["hasSchdule"].GetBool();
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
	auto win = MainWin::Get();
	if (win->hwnd) {
		win->Refresh();
	}
}
