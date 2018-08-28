#pragma once
#include "pro/scode/shortname.hpp"
#include "pro/crypto/publickey.hpp"

#include "blockchain_types.hpp"

namespace chain
{
	using namespace pro;

	struct builder_info
	{
		ShortName		builder_name;
		PublicKey		block_signing_key;

		builder_info(const ShortName& name, const PublicKey& key)
			: builder_name(name)
			, block_signing_key(key)
		{

		}
		builder_info()
		{

		}
	};

	inline bool operator == (const builder_info& lh, const builder_info& rh)
	{
		return (lh.builder_name == rh.builder_name) && (lh.block_signing_key == rh.block_signing_key);
	}

	inline bool operator != (const builder_info& lh, const builder_info& rh)
	{
		return (lh.builder_name != rh.builder_name) || (lh.block_signing_key != rh.block_signing_key);
	}

	typedef std::vector<builder_info> xmax_builder_infos;

	struct builder_rule
	{
	public:
		uint32_t			version;   ///< sequentially incrementing version number
		xmax_builder_infos  builders;

		inline builder_rule()
		{
			version = 0;
		}

		void set_builders(const xmax_builder_infos& list, uint32_t vers)
		{
			builders = list;
			version = vers;
		}

		void reset()
		{
			builders.clear();
			version = 0;
		}

		inline size_t number() const
		{
			return builders.size();
		}

		inline bool is_empty() const
		{
			return builders.size() <= 0;
		}

		PublicKey get_sign_key(const ShortName& name) const
		{
			for (const auto& it : builders)
			{
				if (it.builder_name == name)
				{
					return it.block_signing_key;
				}
			}
			return PublicKey::emptyKey_;
		}
	};

	inline bool operator == (const builder_rule& a, const builder_rule& b)
	{
		if (a.version != b.version) return false;
		if (a.builders.size() != b.builders.size()) return false;
		for (uint32_t i = 0; i < a.builders.size(); ++i)
			if (a.builders[i] != b.builders[i]) return false;
		return true;
	}

	inline bool operator != (const builder_rule& a, const builder_rule& b)
	{
		return !(a == b);
	}

	struct  mapped_builder_rule
	{
		mapped_builder_rule(const chain::allocator<char>& alloc)
			: builders_(alloc) {}


		uint32_t                                        version_ = 0; ///< sequentially incrementing version number
		chain::mapped_vector<builder_info>				builders_;
	};
}