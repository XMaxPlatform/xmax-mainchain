#include "netutl.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/host_name.hpp>
#include <pro/log/log.hpp>
#include <set>

namespace xmax
{
	using boost::asio::ip::tcp;
	using boost::asio::ip::address_v4;
	using boost::asio::ip::host_name;
	using boost::asio::ip::address;

	std::vector<std::string> GetPublicPrivateAddress()
	{
		std::vector<std::string> addr_list;

#if defined(_WIN32)
		WSAData wsaData;
		if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
		{
			ErrorSprintf("No Network!\n");
		}

		char ac[80];
		if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR)
		{
			WSACleanup();
			return addr_list;
		}

		struct hostent* phe = gethostbyname(ac);
		if (phe == 0)
		{
			Logf("Bad host lookup\n");
			WSACleanup();
			return addr_list;
		}

		for (int i = 0; phe->h_addr_list[i] != 0; ++i)
		{
			struct in_addr addr;
			memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
			std::string addrStr = std::string(inet_ntoa(addr));
			if (!IsLocalAddress(addrStr))
			{
				addr_list.push_back(addrStr);
			}
		}

		WSACleanup();
#endif
		return addr_list;
	}

	bool IsLocalAddress(const std::string& addr)
	{
		address bi_addr = address::from_string(addr);

		static const std::set<address> kRejectAddresses = {
			{ address_v4::from_string("127.0.0.1") },
		{ address_v4::from_string("0.0.0.0") },
		};

		return std::find(kRejectAddresses.begin(), kRejectAddresses.end(), bi_addr) != kRejectAddresses.end();
	}

	bool IsPrivateAddress(const std::string& addr)
	{
		address bi_addr = address::from_string(addr);

		if (bi_addr.is_v4())
		{
			address_v4 v4Address = bi_addr.to_v4();
			address_v4::bytes_type bytes_to_check = v4Address.to_bytes();
			if (bytes_to_check[0] == 10 || bytes_to_check[0] == 127)
			{
				return true;
			}			
			if (bytes_to_check[0] == 172 && (bytes_to_check[1] >= 16 && bytes_to_check[1] <= 31))
			{
				return true;
			}				
			if (bytes_to_check[0] == 192 && bytes_to_check[1] == 168)
			{
				return true;
			}				
		}
		else if (bi_addr.is_v6())
		{
			Logf("not supported ipv6\n");
		}


		return false;
	}

	bool IsPublicAddress(const std::string& addr)
	{
		return !(IsPrivateAddress(addr) || IsLocalAddress(addr));
	}
}