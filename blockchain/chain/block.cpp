#include "block.hpp"
#include "chain_utils.hpp"

namespace chain
{
	HashDigest block_header::Digest() const
	{
		HashDigest sha;
		sha.Hash(*this);
		return sha;
	}

	uint32_t block_header::Block_Num() const
	{
		return utils::num_from_id(previous_) + 1;
	}
}