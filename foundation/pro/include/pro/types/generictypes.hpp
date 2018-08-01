/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/build.hpp>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
namespace pro
{
	/**
	* typedef of string, using stl version
	*/
	using string = std::string;
	/**
	* typedef of uint8, using stl version
	*/
	using uint8 = std::uint8_t;
	/**
	* typedef of uint16, using stl version
	*/
	using uint16 = std::uint16_t;
	/**
	* typedef of uint32, using stl version
	*/
	using uint32 = std::uint32_t;
	/**
	* typedef of uint64, using stl version
	*/
	using uint64 = std::uint64_t;
	/**
	* typedef of int8, using stl version
	*/
	using int8 = std::int8_t;
	/**
	* typedef of int16, using stl version
	*/
	using int16 = std::int16_t;
	/**
	* typedef of int32, using stl version
	*/
	using int32 = std::int32_t;
	/**
	* typedef of int64, using stl version
	*/
	using int64 = std::int64_t;

	using uint128 = boost::multiprecision::uint128_t;

	using int128 = boost::multiprecision::int128_t;

	using int256 = boost::multiprecision::int256_t;

}

#define SIZE_MB (1024 * 1024)