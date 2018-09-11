/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/build.hpp>
#include <chrono>
namespace pro
{
	namespace tch = std::chrono;
	/**
	*   Implementation of Seconds class
	*/
	using TimeSeconds = tch::seconds;

	/**
	*  Implementation of Millisecond class
	*/
	using TimeMilliseconds = tch::milliseconds;

	/**
	*  Implementation of Microsecond class
	*/
	using TimeMicroseconds = tch::microseconds;


	using TimePoint = tch::time_point<tch::system_clock, TimeMicroseconds>;

}


