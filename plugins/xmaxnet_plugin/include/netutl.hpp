#pragma once

#include <string>
#include <vector>

namespace xmax
{
	std::vector<std::string> GetPublicPrivateAddress();
	std::vector<std::string> GetLocalAddress();

	bool IsLocalAddress(const std::string& addr);

	bool IsPrivateAddress(const std::string& addr);

	bool IsPublicAddress(const std::string& addr);
}