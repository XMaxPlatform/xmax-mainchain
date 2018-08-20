#include "pro/crypto/publickey.hpp"
#include "pro/crypto/secp256_util.hpp"

namespace pro
{

bool PublicKey::Verify(const CSHA256& hash, const std::vector<unsigned char>& vchSig) const
{
	if (!IsValid())
		return false;
	secp256k1_pubkey pubkey;
	secp256k1_ecdsa_signature sig;
	if (!secp256k1_ec_pubkey_parse(Secp256Context::GetInstance().GetVerifyContext(), &pubkey, &(*this)[0], Size())) 
	{
		return false;
	}
	if (vchSig.size() == 0) 
	{
		return false;
	}
	if (!Secp256Context::GetInstance().ecdsa_signature_parse_der_lax(Secp256Context::GetInstance().GetVerifyContext(), &sig, &vchSig[0], vchSig.size())) 
	{
		return false;
	}
	/* libsecp256k1's ECDSA verification requires lower-S signatures, which have
	* not historically been enforced in Bitcoin, so normalize them first. */
	secp256k1_ecdsa_signature_normalize(Secp256Context::GetInstance().GetVerifyContext(), &sig, &sig);
	return secp256k1_ecdsa_verify(Secp256Context::GetInstance().GetVerifyContext(), &sig, hash.Data(), &pubkey);
}

bool PublicKey::RecoverCompact(const CSHA256 &hash, const std::vector<unsigned char>& vchSig)
{
	if (vchSig.size() != 65)
		return false;
	int recid = (vchSig[0] - 27) & 3;
	bool fComp = ((vchSig[0] - 27) & 4) != 0;
	secp256k1_pubkey pubkey;
	secp256k1_ecdsa_recoverable_signature sig;
	if (!secp256k1_ecdsa_recoverable_signature_parse_compact(Secp256Context::GetInstance().GetVerifyContext(), &sig, &vchSig[1], recid)) 
	{
		return false;
	}
	if (!secp256k1_ecdsa_recover(Secp256Context::GetInstance().GetVerifyContext(), &pubkey, &sig, hash.Data())) 
	{
		return false;
	}
	unsigned char pub[65];
	size_t publen = 65;
	secp256k1_ec_pubkey_serialize(Secp256Context::GetInstance().GetVerifyContext(), pub, &publen, &pubkey, fComp ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED);
	Set(pub, pub + publen);
	return true;
}

bool PublicKey::IsFullyValid() const
{
	if (!IsValid())
		return false;
	secp256k1_pubkey pubkey;
	return secp256k1_ec_pubkey_parse(Secp256Context::GetInstance().GetVerifyContext(), &pubkey, &(*this)[0], Size());
}

bool PublicKey::Decompress()
{
	if (!IsValid())
		return false;
	secp256k1_pubkey pubkey;
	if (!secp256k1_ec_pubkey_parse(Secp256Context::GetInstance().GetVerifyContext(), &pubkey, &(*this)[0], Size())) 
	{
		return false;
	}
	unsigned char pub[65];
	size_t publen = 65;
	secp256k1_ec_pubkey_serialize(Secp256Context::GetInstance().GetVerifyContext(), pub, &publen, &pubkey, SECP256K1_EC_UNCOMPRESSED);
	Set(pub, pub + publen);
	return true;
}

}