#pragma once

#include <picosha2.h>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>

namespace pro
{
	class CSHA256
{
public:

	CSHA256();

	~CSHA256();

	template<typename T> void Hash(const T& src);

	const std::string& GetHex() const;

	const char*	Data() const;

private:
	
	mutable std::string		hexHashStr_;
	std::vector<char>		hash_;
};

template<typename T>
void CSHA256::Hash(const T& src)
{
	std::stringbuf sbuf;
	std::ostream os(&sbuf);
	cereal::BinaryOutputArchive archive(os);
	archive(CEREAL_NVP(src));

	size_t nSize = sbuf.pubseekoff(0, std::ios_base::end);
	sbuf.pubseekoff(0, std::ios_base::beg);
	std::vector<char> vec(nSize);
	sbuf.sgetn(&vec[0], nSize);

	picosha2::hash256(vec.begin(), vec.end(), hash_.begin(), hash_.end());

}

inline const std::string& CSHA256::GetHex() const
{
	if (hexHashStr_.empty())
	{
		hexHashStr_ = picosha2::bytes_to_hex_string(hash_.begin(), hash_.end());
	}

	return hexHashStr_;
}

inline const char* CSHA256::Data() const
{
	return &hash_[0];
}

}