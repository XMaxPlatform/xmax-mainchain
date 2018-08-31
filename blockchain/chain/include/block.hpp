#pragma once
#include <vector>
#include <memory>
#include <optional>

#include "builder_rule.hpp"
#include <chain_types.hpp>

namespace chain
{
	using namespace pro;

	class block_header
	{
	public:

		REFLECT_MEMBER_SERIALIZATION((previous_)(trxs_mroot_)(builder_)(next_builders_));

		CSHA256							Digest() const;
		uint32_t						Block_num() const;

		CSHA256							previous_;
		CSHA256							trxs_mroot_;
		ShortName						builder_;
		std::optional<builder_rule>		next_builders_;
	};

	class signed_block_header : public block_header
	{
	public:
	};

	class transaction_receipt_header
	{
	public:

		enum result_code : uint8_t
		{
			applied = 0,
			failure = 1,
		};

		uint64_t								receipt_idx;
	};

	class transaction_receipt : public transaction_receipt_header
	{
	public:
	};

	class signed_block : public signed_block_header
	{
	public:
		std::vector<transaction_receipt> receipts_;
	};

	class signed_block_list
	{
	public:
		std::vector<signed_block>	blockList_;
	};

	using signed_block_ptr = std::shared_ptr<signed_block>;

	class block_confirmation_header
	{
	public:

		CSHA256				block_id_;
		ShortName			verifier_;

		CSHA256				Digest() const;
	};

	class block_confirmation : public block_confirmation_header
	{

	};
}