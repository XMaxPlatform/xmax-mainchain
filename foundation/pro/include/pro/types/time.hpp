/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/build.hpp>
namespace pro
{
#define TIME_GENERAL(_FracOfSecond) \
	public:\
		using TimeBase::TimeBase;\
		static const int64_t FracOfSecond = _FracOfSecond;\
	private:

/*!
 * \class TimeBase
 * the base class of time.
 */
	template<typename _Time>
	class TimeBase
	{
	public:
		typedef int64_t TimeType;
		TimeBase()
			: time_(0)
		{
		}
		TimeBase(TimeType val)
			: time_(val)
		{
		}
		/**
		* Convert to another type of time base on the FracOfSeconds.
		* @return[out] time 'To'
		*/
		template<typename To>
		To ToTime() const
		{
			if constexpr (To::FracOfSecond > _Time::FracOfSecond)
			{
				return To(time_ * (To::FracOfSecond / _Time::FracOfSecond));
			}
			return To(time_ / (_Time::FracOfSecond / To::FracOfSecond));
		}

		/**
		* Get the time value.
		* @return[out] time value.
		*/
		TimeType GetValue() const
		{
			return time_;
		}

	private:

		TimeType time_;
	};

	class TimeSeconds : public TimeBase<TimeSeconds>
	{
		TIME_GENERAL(1);

	};
	class TimeMilliseconds : public TimeBase<TimeMilliseconds>
	{
	public:
		TIME_GENERAL(1000);
	};

	class TimeMicroseconds : public TimeBase<TimeMicroseconds>
	{
	public:
		TIME_GENERAL(1000000);
	};
}


