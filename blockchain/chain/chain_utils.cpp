#include "chain_utils.hpp"
#include "pro/utils/bitutil.hpp"

namespace chain
{
	uint32_t num_from_id(const BlockID& id)
	{
		uint64_t ut;
		memcpy(&ut, id.Data(), 64);
		return pro::endian_reverse_u32(ut);
	}
}