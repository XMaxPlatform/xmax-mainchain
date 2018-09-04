#pragma once
#include "pro/scode/shortname.hpp"
#include "pro/crypto/publickey.hpp"
#include "pro/utils/reflect.hpp"
#include <chain_types.hpp>

namespace chain
{
	struct builder_info
	{
		Name			builder_name;
		PublicKey		block_signing_key;

		builder_info(const Name& name, const PublicKey& key)
			: builder_name(name)
			, block_signing_key(key)
		{

		}
		builder_info()
		{

		}

		REFLECT_MEMBER_SERIALIZATION( (builder_name) (block_signing_key) )
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

		REFLECT_MEMBER_SERIALIZATION((version)(builders))

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

		PublicKey get_sign_key(const Name& name) const
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

		mapped_builder_rule& operator=(const builder_rule& a) {
			version_ = a.version;
			builders_.clear();
			builders_.reserve(a.builders.size());
			for (const auto& p : a.builders)
				builders_.push_back(p);
			return *this;
		}

		operator builder_rule() const {
			builder_rule result;
			result.version = version_;
			result.builders.reserve(builders_.size());
			for (const auto& p : builders_)
				result.builders.push_back(p);
			return result;
		}

		void set_builders(const xmax_builder_infos& list, uint32_t vers)
		{
			builders_.clear();
			builders_.assign(list.begin(), list.end());
			version_ = vers;
		}
		void set_builders(const mapped_vector<builder_info>& list, uint32_t vers)
		{
			builders_ = list;
			version_ = vers;
		}		void reset()
		{
			builders_.clear();
			version_ = 0;
		}
		inline int number() const
		{
			return builders_.size();
		}

		inline bool is_empty() const
		{
			return builders_.size() <= 0;
		}

		uint32_t                                        version_ = 0; ///< sequentially incrementing version number
		chain::mapped_vector<builder_info>				builders_;
	};
}