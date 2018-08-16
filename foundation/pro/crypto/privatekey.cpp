#include "pro/crypto/privatekey.hpp"
#include "pro/crypto/Base58.hpp"
#include "pro/crypto/secp256_util.hpp"
#include "pro/utils/sha256.hpp"

namespace pro
{
PrivateKey::PrivateKey()
	: bValid_(false),
	  bCompressed_(false)
{
	keyData_.resize(32);
}

PrivateKey::~PrivateKey()
{

}

PrivateKey PrivateKey::FromBase58string(const Base58String& str)
{
	Base58Data data;
	bool fGood = data.SetBase58string(str);
	if (!fGood)
		return PrivateKey(); // invalid key

							
	bool fExpectedFormat =
		data.vchData_.size() == 32
		|| (data.vchData_.size() == 33 && data.vchData_[32] == 1);
	if (!fExpectedFormat)
		return PrivateKey(); // invalid key

										
	bool fCorrectVersion =
		data.vchVersion_ == std::vector<unsigned char>(1, 128);
	if (!fCorrectVersion)
		return PrivateKey(); // invalid key;

									
	PrivateKey ret;
	assert(data.vchData_.size() >= 32);

	ret.SetBinary(
		data.vchData_.data(),      // pbegin
		data.vchData_.data() + 32, // pend
		data.vchData_.size() > 32 && data.vchData_[32] == 1 // compressed
	);
	return ret;
}

PublicKey PrivateKey::GetPubKey() const
{
	secp256k1_pubkey pubkey;
	size_t clen = 65;
	PublicKey result;
	int ret = secp256k1_ec_pubkey_create(Secp256Context::GetInstance().GetSecpContext(), &pubkey, Begin());

	secp256k1_ec_pubkey_serialize(
		Secp256Context::GetInstance().GetSecpContext(),
		(unsigned char*)result.Begin(),
		&clen,
		&pubkey,
		bCompressed_ ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED
	);
	
	return result;
}

bool PrivateKey::Sign(const CSHA256& hash, std::vector<unsigned char>& vchSig) const
{
	if (!bValid_)
		return false;
	vchSig.resize(72);
	size_t nSigLen = 72;
	
	secp256k1_ecdsa_signature sig;
	int ret = secp256k1_ecdsa_sign(Secp256Context::GetInstance().GetSecpContext(), &sig, hash.Data(), Begin(), 
								   secp256k1_nonce_function_rfc6979, NULL);
	
	secp256k1_ecdsa_signature_serialize_der(Secp256Context::GetInstance().GetSecpContext(), (unsigned char*)&vchSig[0], &nSigLen, &sig);
	vchSig.resize(nSigLen);
	return true;
}

bool PrivateKey::VerifyPublicKey(const PublicKey& pubkey) const
{
	if (pubkey.IsCompressed() != bCompressed_) 
	{
		return false;
	}
	
	std::string str = "xmax key verification\n";
	CSHA256 sha;
	std::vector<unsigned char> vec;
	vec.assign(str.begin(), str.end());
	sha.Hash(vec.begin(), vec.end());
	std::vector<unsigned char> vchSig;
	Sign(sha, vchSig);
	return pubkey.Verify(sha, vchSig);
}

bool PrivateKey::SignCompact(const CSHA256& hash, std::vector<unsigned char>& vchSig) const
{
	if (!bValid_)
	{
		return false;
	}

	vchSig.resize(65);
	int rec = -1;
	secp256k1_ecdsa_recoverable_signature sig;
	int ret = secp256k1_ecdsa_sign_recoverable(Secp256Context::GetInstance().GetSecpContext(), &sig, 
											   hash.Data(), Begin(), secp256k1_nonce_function_rfc6979, NULL);
	
	secp256k1_ecdsa_recoverable_signature_serialize_compact(Secp256Context::GetInstance().GetSecpContext(), (unsigned char*)&vchSig[1], &rec, &sig);
	vchSig[0] = 27 + rec + (bCompressed_ ? 4 : 0);
	return true;
}

bool PrivateKey::Load(std::vector<unsigned char>& privkey, PublicKey& vchPubKey, bool bSkipCheck)
{
	if (!Secp256Context::GetInstance().EC_PrivateKey_Import(Secp256Context::GetInstance().GetSecpContext(), (unsigned char*)Begin(), &privkey[0], privkey.size()))
		return false;
	bCompressed_ = vchPubKey.IsCompressed();
	bValid_ = true;

	if (bSkipCheck)
		return true;

	return VerifyPublicKey(vchPubKey);
}



}