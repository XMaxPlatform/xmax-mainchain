#include "pro/crypto/Base58String.hpp"

namespace pro
{
Base58String::Base58String(const std::string& str, unsigned int nVersionBytes /* = 1 */)
{
	Base58Data data;
	data.SetStringWithVersionBytes(str.c_str(), nVersionBytes);
	string_ = data.ToString();
}

Base58String::Base58String(const char* sz, unsigned int nVersionBytes /* = 1 */)
{
	Base58Data data;
	data.SetStringWithVersionBytes(sz, nVersionBytes);
	string_ = data.ToString();
}

void Base58String::SetData(const std::vector<unsigned char>& vchVersionIn, const void* pdata, size_t nSize)
{
	Base58Data data;
	data.SetData(vchVersionIn, pdata, nSize);
	string_ = data.ToString();
}

void Base58String::SetData(const std::vector<unsigned char>& vchVersionIn, const unsigned char* pbegin, const unsigned char* pend)
{
	Base58Data data;
	data.SetData(vchVersionIn, pbegin, pend);
	string_ = data.ToString();
}

}