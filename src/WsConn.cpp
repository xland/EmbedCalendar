#include <string>
#include <Windows.h>
#include <format>
#include <rapidjson/document.h>

#include "Util.h"
#include "WsConn.h"
#include "MainWin.h"
#include "Skin.h"
#include "DateItem.h"
#include "Ctrl/CalendarHeader.h"
#include "Ctrl/WeekHeader.h"
#include "Ctrl/CalendarBody.h"
#include "Ctrl/ListHeader.h"

namespace {
	std::unique_ptr<WsConn> wsConn;
}

WsConn::WsConn()
{
}

WsConn::~WsConn()
{
}

void WsConn::Init()
{
	wsConn = std::make_unique<WsConn>();
	wsConn->initJson();
}

WsConn* WsConn::Get()
{
	return wsConn.get();
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
"displayScheduleList":true,"scheduleList":[{"title":"日程标题日程标题11","desc":"日程摘要xxx11","isAllowEdit":false,"calendarColor":"#000000", "calendarNo":"xxxx","scheduleNo":"yyyy"},
{"title":"日程标题日程标题22","desc":"日程摘要xxx22","isAllowEdit":true,"calendarColor":"#000000", "calendarNo":"xxxx22","scheduleNo":"yyyy22"}],)" };
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
		static std::string currtStr = Util::ToStr(L"currt");
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
	auto win = MainWin::Get();
	if (win->hwnd) {
		win->Refresh();
	}
}
