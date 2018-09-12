/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <chaintypes.hpp>
#include <chain_db.hpp>
#include <block_content.hpp>

namespace chain
{
	struct PendingBlock
	{
		UndoPatch undopatch;
		BlockContentPtr content;

		PendingBlock(UndoPatch&& p)
			: undopatch(std::forward<UndoPatch>(p))
		{
		}
	};
}