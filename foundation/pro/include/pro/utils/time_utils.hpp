/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <pro/types/generictypes.hpp>
#include <pro/types/time.hpp>

namespace pro
{
	namespace utils
	{
		// format hour::Munite::Seconds.milliseconds
		string TimeNowDateMSString();

		TimePoint TimeNow();
	}
}


