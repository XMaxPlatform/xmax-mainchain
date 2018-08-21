#pragma once

#include "Base58.hpp"

namespace pro
{
	class Base58String
{
public:
	explicit Base58String(const std::string& str, unsigned int nVersionBytes = 1);
	explicit Base58String(const char* sz, unsigned int nVersionBytes = 1);

	void SetData(const std::vector<unsigned char>& vchVersionIn, const void* pdata, size_t nSize);
	void SetData(const std::vector<unsigned char>& vchVersionIn, const unsigned char* pbegin, const unsigned char* pend);

	const char* c_str() const;
	size_t length() const;

	int CompareTo(const Base58String& str) const;
	bool operator==(const Base58String& str) const { return CompareTo(str) == 0; }
	bool operator<=(const Base58String& str) const { return CompareTo(str) <= 0; }
	bool operator>=(const Base58String& str) const { return CompareTo(str) >= 0; }
	bool operator< (const Base58String& str) const { return CompareTo(str) < 0; }
	bool operator> (const Base58String& str) const { return CompareTo(str) > 0; }

protected:
private:

	std::string		string_;
};

inline const char* Base58String::c_str() const
{
	return string_.c_str();
}

inline size_t Base58String::length() const
{
	return string_.length();
}

inline int Base58String::CompareTo(const Base58String& str) const
{
	return string_.compare(str.string_);
}

}