#include <block.hpp>
#include <chain_utils.hpp>

namespace chain
{
	HashDigest BlockHeader::Digest() const
	{
		HashDigest sha;
		sha.Hash(*this);
		return sha;
	}

	uint32_t BlockHeader::Block_Num() const
	{
		return utils::num_from_id(previous_) + 1;
	}

	BlockID SignedBlockHeader::Id() const
	{
		return utils::block_id(Digest(), Block_Num());
	}

	
}