/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/build.hpp>
namespace pro
{

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

		template<typename To>
		To ToTime() const
		{
			if constexpr (To::FracOfSecond > _Time::FracOfSecond)
			{
				return To(time_ * (To::FracOfSecond / _Time::FracOfSecond));
			}
			return To(time_ / (_Time::FracOfSecond / To::FracOfSecond));
		}

		TimeType GetValue() const
		{
			return time_;
		}

	private:

		TimeType time_;
	};

	class TimeSeconds : public TimeBase<TimeSeconds>
	{
	public:
		using TimeBase::TimeBase;
		static const int64_t FracOfSecond = 1;

	};
	class TimeMilliseconds : public TimeBase<TimeMilliseconds>
	{
	public:
		using TimeBase::TimeBase;
		static const int64_t FracOfSecond = 1000;
	};

	class TimeMicroseconds : public TimeBase<TimeMicroseconds>
	{
	public:
		using TimeBase::TimeBase;
		static const int64_t FracOfSecond = 1000000;
	};
}


