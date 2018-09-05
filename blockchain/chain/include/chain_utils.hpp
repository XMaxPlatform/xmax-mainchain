/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <chain_types.hpp>

namespace chain
{
	namespace utils
	{
		uint32_t num_from_id(const BlockID& id);

		BlockID block_id(const BlockID& summary, uint32_t num);

		HashDigest cal_merkle(std::vector<HashDigest> digests);
	}
}
