#pragma once
#include <vector>
#include <memory>
#include <optional>

#include "builder_rule.hpp"
#include "pro/crypto/privatekey.hpp"
#include <chain_types.hpp>

namespace chain
{
	class block_header
	{
		RF_BODY(
			(RF_SFIELD(BlockID, previous_))
			(RF_SFIELD(HashDigest, trxs_mroot_))
			(RF_SFIELD(Name, builder_))
			(RF_SFIELD(std::optional<builder_rule>, next_builders_))
		)
	public:
		HashDigest Digest() const;
	};

	class signed_block_header : public block_header
	{
		RF_BODY_INHERIT(block_header,
			(RF_SFIELD(Signature, builder_signature_))
		)
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

		HashDigest		Cal_Digest() const;
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

		BlockID			block_id_;
		Name			verifier_;

	};

	class block_confirmation : public block_confirmation_header
	{
	public:

		Signature		builder_signature_;
	};
}