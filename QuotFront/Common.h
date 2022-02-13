#pragma once
#ifndef _COMMON_H_
#define _COMMON_H_
#include <future>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <windows.h>
#include <time.h>
#include <set>
#include <unordered_map>
#include <unordered_set>


#define  MAX_RECENT_MSGS     100                       // 缓存得最新msg个数
#define  RECONNECT_TIME_OUT  10000000000               // 重连超时时间纳秒
#define  RECONNECT_TIMES     15                        // 重连次数
#define  DELAY_TIME          1000000                   //delay时间纳秒
#define  CLIENT_MSG_HEADER   "{(len="                  //delay时间纳秒
#define  MARKETRSPPREX       "MARKET01@@@@@@&"         //订阅信息前缀



using namespace std;
typedef uint64_t uint64;

namespace HP
{
	inline void MayaSplit(const string& maya, unordered_map<string, bool>& arSubMap, bool isSub, char ch = ';')
	{
		string sKeyContract;
		string sKeyReplaceContract;
		size_t count = maya.length();
		int iPos = -1;
		for (size_t i = 0; i < count; i++)
		{
			if (maya[i] == ch)
			{
				// replace the "," to "@"
				iPos = sKeyContract.find(',');
				sKeyReplaceContract = sKeyContract.replace(iPos, 1, "@");

				// +
				if (isSub)
				{
					arSubMap[sKeyReplaceContract] = true;
				}
				//-
				else if (arSubMap.find(sKeyReplaceContract) != arSubMap.end())
				{
					arSubMap[sKeyReplaceContract] = false;
				}

				sKeyContract.clear();
				sKeyReplaceContract.clear();
			}
			else
			{
				sKeyContract += maya[i];
			}
		}		 
	}
}

namespace LocalMarket
{
	struct DepthData {
		double Price;
		unsigned int Qty;

		DepthData()
		{
			Price = 0;
			Qty = 0;
		}
	};
	
	inline void getFormatByNanSec(const time_t timestamp, char* sFormatTime)
	{
		time_t newSec = timestamp / (1000 * 1000 * 1000);
		struct tm t;
		if (newSec) {
#ifdef _WIN32
			localtime_s(&t, &newSec);
#else
			localtime_r(&pData->LastTime, &t);
#endif
			sprintf(sFormatTime, "%d-%02d-%02d %02d:%02d:%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
		}
	};
}
#endif