#pragma once

#include "builder_rule.hpp"
#include "pro/types/generictypes.hpp"

namespace chain
{
	uint32_t num_from_id(const BlockID& id);

	CSHA256 block_id(const CSHA256& summary, uint32_t num);
}
