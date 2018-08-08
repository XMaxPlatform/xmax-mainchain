#pragma once
#include <vector>
#include <memory>

namespace chain
{
	class block_header
	{
	public:
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