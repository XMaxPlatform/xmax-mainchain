#pragma once
#include <vector>
#include <memory>
#include <optional>

#include "builder_rule.hpp"
#include "pro/crypto/privatekey.hpp"
#include <chain_types.hpp>

namespace chain
{
	using namespace pro;

	class block_header
	{
	public:

		//REFLECT_MEMBER_SERIALIZATION((previous_)(trxs_mroot_)(builder_)(next_builders_));

		CSHA256							Digest() const;
		uint32_t						Block_num() const;

		RF_BODY(
			(RF_SFIELD(CSHA256, previous_))
			(RF_SFIELD(CSHA256, trxs_mroot_))
			(RF_SFIELD(ShortName, builder_))
			(RF_SFIELD(std::optional<builder_rule>, next_builders_))
		)			
	};

	class signed_block_header : public block_header
	{
	public:

		CSHA256			Id() const;
		PublicKey		Get_signer_key() const;
		void			Sign(const PrivateKey& signer);
		bool			Is_signer_valid(const PublicKey& signer_key) const;

		std::array<unsigned char, 65>	builder_signature_;
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

		CSHA256		Cal_Digest() const;
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
	public:

		std::array<unsigned char, 65>		builder_signature_;
		PublicKey							Get_Signer_Key() const;
		void								Sign(const PrivateKey& signer);
		bool								Is_Signer_Valid(const PublicKey &signer_key) const;

		static block_confirmation Make_Conf(const CSHA256& id, const ShortName& account, const PrivateKey& validate_private_key);
	};
}