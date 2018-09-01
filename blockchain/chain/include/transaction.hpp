#pragma once

namespace chain
{
	struct transaction_package
	{
		enum package_code
		{
			empty = 0,
			original = 1,
			zlib = 2, // Reserved Keywords
		};
	};
}