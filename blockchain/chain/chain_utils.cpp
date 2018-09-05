#include "chain_utils.hpp"
#include "pro/utils/bitutil.hpp"


namespace chain
{
	namespace utils
	{
		uint32_t num_from_id(const BlockID& id)
		{
			uint64_t ut;
			memcpy(&ut, id.Data(), 64);
			return pro::endian_reverse_u32(ut);
		}

		BlockID block_id(const BlockID& summary, uint32_t num)
		{
			uint64_t ut;
			memcpy(&ut, summary.Data(), 64);
			ut &= 0xffffffff00000000;
			ut += pro::endian_reverse_u32(num);
			BlockID ret(summary.GetDataVec());
			memcpy(&ret.GetDataVec()[0], &ut, 64);
			return ret;
		}

		HashDigest cal_merkle(std::vector<HashDigest> digests)
		{
			if (digests.size() == 0)
			{
				return HashDigest();
			}
			while (digests.size() > 1)
			{
				if (digests.size() % 2)
				{
					digests.push_back(digests.back());
				}
				for (int i = 0; i < digests.size() / 2; ++i)
				{
					HashDigest sha;
					sha.Hash(std::make_pair(digests[i], digests[i + 1]));
					digests[i / 2] = sha;
				}
				digests.resize(digests.size() / 2);
			}
			return digests.front();
		}
	}
}