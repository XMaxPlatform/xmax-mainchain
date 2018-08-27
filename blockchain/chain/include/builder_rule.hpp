#pragma once
#include "pro/scode/shortname.hpp"
#include "pro/crypto/publickey.hpp"


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
}