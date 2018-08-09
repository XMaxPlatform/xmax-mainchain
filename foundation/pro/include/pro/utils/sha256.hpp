#pragma once

#include <picosha2.h>

namespace pro
{
	class CSHA256
{
public:

	std::string Hash(const std::vector<char>& src);

protected:

private:
	
};

inline std::string CSHA256::Hash(const std::vector<char>& src)
{
	std::vector<char> hash(picosha2::k_digest_size);
	picosha2::hash256(src.begin(), src.end(), hash.begin(), hash.end());

	std:: string hexStr = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
	return hexStr;
}

}