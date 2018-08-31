#pragma once
#include <vector>
#include <memory>
#include <optional>

#include "pro/utils/sha256.hpp"
#include "pro/utils/reflect.hpp"
#include <blockchain_types.hpp>

namespace chain
{
	class block_header
	{
	public:

		REFLECT_MEMBER_SERIALIZATION( (previous_)(trxs_mroot_))

		CSHA256			previous_;
		CSHA256			trxs_mroot_;
		//std::optional<>
	};

	class signed_block_header : public block_header
	{
	public:
	};

	class transaction_receipt_header
	{
	public:
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

	};

	class block_confirmation : public block_confirmation_header
	{

	};
}