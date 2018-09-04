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

	CSHA256 block_id(const CSHA256& summary, uint32_t num)
	{
		uint64_t ut;
		memcpy(&ut, summary.Data(), 64);
		ut &= 0xffffffff00000000;
		ut += pro::endian_reverse_u32(num);
		CSHA256 ret(summary.GetDataVec());
		memcpy(&ret.GetDataVec()[0], &ut, 64);
		return ret;
	}
}