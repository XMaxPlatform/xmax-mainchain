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
		DataPatch undopatch;
		BlockContentPtr content;

		PendingBlock(DataPatch&& p)
			: undopatch(std::forward<DataPatch>(p))
		{
		}
		void PushDB()
		{
			undopatch.Push();
		}
	};
}