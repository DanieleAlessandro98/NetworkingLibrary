#pragma once

#include "StdAfx.h"
#include <cstdint>
#include <time.h>

namespace Net
{
	class Utils
	{
		public:
			static void GetTimeDay(struct timeval* t, struct timezone* dummy)
			{
				uint32_t millisec = GetTickCount();

				t->tv_sec = (millisec / 1000);
				t->tv_usec = (millisec % 1000) * 1000;
			}

			static uint32_t GetBootSec()
			{
				static struct timeval tv_boot = { 0L, 0L };

				if (tv_boot.tv_sec == 0)
					GetTimeDay(&tv_boot, NULL);

				return tv_boot.tv_sec;
			}

			static uint32_t GetTime()
			{
				struct timeval tv;
				GetTimeDay(&tv, NULL);
				tv.tv_sec -= GetBootSec();
				return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
			}

			static uint32_t GenerateCRC32(const char* buf, size_t len)
			{
				uint32_t crc = 0;
				for (size_t i = 0; i < len; ++i) {
					crc += static_cast<uint32_t>(buf[i]);
				}
				return crc;
			}

			static uint32_t GenerateRandomNumber()
			{
				return rand() % (1024 * 1024);
			}

			static uint32_t GetGlobalTime()
			{
				return static_cast<uint32_t>(time(0));
			}
	};
}
