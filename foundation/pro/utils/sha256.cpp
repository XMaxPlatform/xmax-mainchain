#include "pro/utils/sha256.hpp"

namespace pro
{
CSHA256::CSHA256()
	: hash_(picosha2::k_digest_size)
{

}

CSHA256::~CSHA256()
{

}

}