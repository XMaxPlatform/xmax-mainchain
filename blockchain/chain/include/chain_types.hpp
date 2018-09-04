#pragma once

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <array>

#include <pro/protypes.hpp>
#include <pro/crypto/privatekey.hpp>
#include <pro/crypto/publickey.hpp>

namespace chain
{
	namespace bip = boost::interprocess;

	template<typename T>
	using allocator = bip::allocator<T, bip::managed_mapped_file::segment_manager>;

	template<typename T>
	using mapped_vector = boost::interprocess::vector<T, allocator<T>>;

	using Signature = std::array<unsigned char, 65>;

	using PublicKey = pro::PublicKey;

	using PrivateKey = pro::PrivateKey;

	using Name = pro::ShortName;

	using HashDigest = pro::CSHA256;

	using HashID = pro::CSHA256;

	using BlockID = HashID;
}
