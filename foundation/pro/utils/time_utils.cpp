/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <chrono>
#include <iostream>

#include <pro/types/generictypes.hpp>
#include <pro/utils/time_utils.hpp>

namespace pro
{
	namespace utils
	{
		namespace chrono = std::chrono;

		string TimeNowDateMSString()
		{
			// Because c-style date&time utilities don't support microsecond precison,
			// we have to handle it on our own.
			auto time_now = chrono::system_clock::now();
			auto duration_in_ms = chrono::duration_cast<chrono::milliseconds>(time_now.time_since_epoch());
			auto ms_part = duration_in_ms - chrono::duration_cast<chrono::seconds>(duration_in_ms);

			tm local_time_now;
			time_t raw_time = chrono::system_clock::to_time_t(time_now);
			_localtime64_s(&local_time_now, &raw_time);

			std::stringstream stream;

			stream << std::put_time(&local_time_now, "%H:%M:%S.")
				<< std::setfill('0') << std::setw(3) << ms_part.count();

			return stream.str();
		}

		TimePoint TimeNow()
		{
			auto time_now = chrono::system_clock::now();
			auto duration_in_us = chrono::duration_cast<TimeMicroseconds>(time_now.time_since_epoch());
			return TimePoint(duration_in_us);
		}
	}
}


