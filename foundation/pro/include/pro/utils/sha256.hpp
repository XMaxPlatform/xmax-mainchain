#pragma once

#include <picosha2.h>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>

namespace pro
{
	class CSHA256
{
public:

template<typename T>
	void Hash(const T& src);

	const std::string& GetHex() const;
private:
	
	std::string			hexHashStr_;
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

	std::vector<char> hash(picosha2::k_digest_size);
	picosha2::hash256(vec.begin(), vec.end(), hash.begin(), hash.end());

	hexHashStr_ = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
}

inline const std::string& CSHA256::GetHex() const
{
	return hexHashStr_;
}

}