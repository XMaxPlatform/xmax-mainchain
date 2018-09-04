#include "block.hpp"

namespace chain
{
	HashDigest block_header::Digest() const
	{
		HashDigest sha;
		sha.Hash(*this);
		return sha;
	}
}