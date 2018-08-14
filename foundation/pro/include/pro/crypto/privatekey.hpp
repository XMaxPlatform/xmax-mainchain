#pragma once

#include <vector>
#include "publickey.hpp"

namespace pro
{
	class Base58String;
	class CSHA256;
	class PrivateKey
{
public:

	PrivateKey();

	~PrivateKey();

	static PrivateKey FromBase58string(const Base58String& str);
	friend bool operator == (const PrivateKey& a, const PrivateKey& b);
	void SetBinary(const unsigned char* pbegin, const unsigned char* pend, bool bCompressedIn);

	size_t KeySize() const;
	const unsigned char* Begin() const;
	const unsigned char* End() const;

	bool IsValid() const;
	bool IsCompressed() const;

	//! Initialize from a serialized OpenSSL private key data.
	bool SetOpenSSLKey(const std::vector<unsigned char>& vchPrivKey, bool bCompressed);
	void MakeNewKey(bool bCompressed);

	/**
	* Convert the private key to a  serialized OpenSSL private key data.
	* This is expensive.
	*/
	std::vector<unsigned char> GetOpenSSLKey() const;

	PublicKey GetPubKey() const;

	/**
	* Create a DER-serialized signature.
	* The test_case parameter tweaks the deterministic nonce.
	*/
	bool Sign(const CSHA256& hash, std::vector<unsigned char>& vchSig) const;

	/**
	* Create a compact signature (65 bytes), which allows reconstructing the used public key.
	* The format is one header byte, followed by two times 32 bytes for the serialized r and s values.
	* The header byte: 0x1B = first key with even y, 0x1C = first key with odd y,
	*                  0x1D = second key with even y, 0x1E = second key with odd y,
	*                  add 0x04 for compressed keys.
	*/
	bool SignCompact(const CSHA256& hash, std::vector<unsigned char>& vchSig) const;

	/**
	* Verify thoroughly whether a private key and a public key match.
	* This is done using a different mechanism than just regenerating it.
	*/
	bool VerifyPublicKey(const PublicKey& vchPubKey) const;

	//! Load private key and check that public key matches.
	bool Load(std::vector<unsigned char>& privkey, PublicKey& vchPubKey, bool bSkipCheck);


protected:
private:

	//! Whether this private key is valid. We check for correctness when modifying the key
	//! data, so fValid should always correspond to the actual state.
	bool		bValid_;
	//! Whether the public key corresponding to this private key is (to be) compressed.
	bool		bCompressed_;
	
	std::vector<unsigned char>		keyData_;
};

inline size_t PrivateKey::KeySize() const
{
	return (bValid_ ? keyData_.size() : 0);
}

inline const unsigned char* PrivateKey::Begin() const
{
	return keyData_.data();
}

inline const unsigned char* PrivateKey::End() const
{
	return keyData_.data() + KeySize();
}

inline bool PrivateKey::IsValid() const
{
	return bValid_;
}

inline bool PrivateKey::IsCompressed() const
{
	return bCompressed_;
}

}
