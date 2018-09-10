/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/build.hpp>
#include <chrono>
namespace pro
{
	/**
	*   Implementation of Seconds class
	*/
	using TimeSeconds = std::chrono::seconds;

	/**
	*  Implementation of Millisecond class
	*/
	using TimeMilliseconds = std::chrono::milliseconds;

	/**
	*  Implementation of Microsecond class
	*/
	using TimeMicroseconds = std::chrono::microseconds;


	using TimePoint = std::chrono::system_clock::time_point;

}


