#pragma once
#include <string>
#include "Common.h"
#include "log.h"
extern LOGGER g_logger;

class CZMQSubAdapter
{
public:
	CZMQSubAdapter(char* szAddr);
	~CZMQSubAdapter();
	int EPGMSubInit();
	int  EPGMSubRecv(char* sRecvBuf);
	void EPGMCleanUp();

public:
	//用来进行纳秒级别的sleep
	inline void xdk_microsecond_delay(uint64_t interval)
	{
		interval *= 1000ULL;
		uint64_t t, t0 = xdk_nanosecond_timestamp();
		do
		{
			t = xdk_nanosecond_timestamp();
		} while (t - t0 < interval);
	};

	//用来获取纳秒级别的时间戳

	uint64_t xdk_nanosecond_timestamp()
	{
		static uint64_t baseline = 0;
		static uint64_t multiplier = 0;
		static LARGE_INTEGER frequency;
		static LARGE_INTEGER base_counter;

		uint64_t timestamp = 0;
		if (baseline == 0) {
			::QueryPerformanceFrequency(&frequency);
			multiplier = 1000000000ULL / frequency.QuadPart;
			FILETIME file_time;
			::GetSystemTimeAsFileTime(&file_time);
			baseline |= file_time.dwHighDateTime;
			baseline <<= 32;
			baseline |= file_time.dwLowDateTime;
			baseline *= 100;
			baseline -= 11644473600000000000ULL;
			::QueryPerformanceCounter(&base_counter);
			timestamp = baseline;
		}
		else {
			LARGE_INTEGER counter;
			::QueryPerformanceCounter(&counter);
			timestamp = baseline + (counter.QuadPart - base_counter.QuadPart) * multiplier;
		}
		return timestamp;
	}

private:
	void* subscriber;
	void* context;

	std::string zmq_address;
	int zmq_rate;
	int zmq_rcvbuf;
	int zmq_rcvhwm;
	int zmq_recover;
	//LOGGER m_logger;
	
};

