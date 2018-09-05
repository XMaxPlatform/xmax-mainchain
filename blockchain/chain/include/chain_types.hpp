/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <array>

#include <pro/protypes.hpp>
#include <pro/crypto/privatekey.hpp>
#include <pro/crypto/publickey.hpp>

namespace chain
{
	using Signature = std::array<unsigned char, 65>;

	using PublicKey = pro::PublicKey;

	using PrivateKey = pro::PrivateKey;

	using Name = pro::ShortName;

	using HashDigest = pro::CSHA256;

	using HashID = pro::CSHA256;

	using BlockID = HashID;
}
