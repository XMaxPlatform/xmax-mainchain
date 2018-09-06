#pragma once
#include <vector>
#include <memory>
#include <optional>

#include <pro/crypto/privatekey.hpp>
#include <chain_types.hpp>

namespace chain
{
	class BlockHeader
	{
		RF_BODY(
			(RF_SFIELD(BlockID, previous_))
			(RF_SFIELD(HashDigest, trxs_mroot_))
			(RF_SFIELD(Name, builder_))
		)
	public:
		HashDigest Digest() const;
		uint32_t   Block_Num() const;
	};

	class SignedBlockHeader : public BlockHeader
	{
		RF_BODY_INHERIT(BlockHeader,
			(RF_SFIELD(Signature, builder_signature_))
		)
	public:
		
		BlockID				Id() const;
	};

	class TransactionReceipt
	{
	public:	
		enum result_code : uint8_t
		{
			applied = 0,
			failure = 1,
		};

		uint64_t								receipt_idx;
	};

	class SignedBlock : public SignedBlockHeader
	{
	public:
		std::vector<TransactionReceipt> receipts_;
	};


	using SignedBlockPtr = std::shared_ptr<SignedBlock>;
}