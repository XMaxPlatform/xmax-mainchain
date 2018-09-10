/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <chaintypes.hpp>

namespace chain
{

	class ChainGlobal
	{
	public:
		const static uint32_t ChainTimestampUnitMS = 1000;
		const static uint32_t ChainTimestampUnitUS = ChainTimestampUnitMS * 1000;
		const static uint32_t MiniNextBlockUS = ChainTimestampUnitUS / 10;

		const static uint64_t ChainTimestampEpochS = 946684800; // year 2000 AD. in seconds.

		const static uint64_t ChainTimestampEpochUS = ChainTimestampEpochS * 1000ll * 1000ll;
	};

}