#include "block.hpp"

namespace chain
{
	CSHA256 block_header::Digest() const
	{
		CSHA256 sha;
		sha.Hash(*this);
		return sha;
	}
}