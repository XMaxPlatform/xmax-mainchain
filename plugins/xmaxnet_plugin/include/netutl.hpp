#pragma once

#include <string>
#include <vector>

namespace xmax
{
	std::vector<std::string> GetExternalAddress();

	bool IsLocalAddress(const std::string& addr);

	bool IsPrivateAddress(const std::string& addr);

	bool IsPublicAddress(const std::string& addr);
}