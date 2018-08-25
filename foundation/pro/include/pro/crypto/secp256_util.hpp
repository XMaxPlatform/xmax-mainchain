#pragma once

#include "pro/utils/singleton.hpp"

#include <secp256k1.h>
#include <secp256k1_recovery.h>

namespace pro
{
	//helper class for secp256k1
	class Secp256Context : public Singleton<Secp256Context>
{
public:
	//import private key
	int EC_PrivateKey_Import(const secp256k1_context* ctx, unsigned char *out32, const unsigned char *privkey, size_t privkeylen);
	//export private key
	int EC_PrivateKey_Export(const secp256k1_context* ctx, unsigned char* privkey, size_t* privkeylen, const unsigned char* key32, int compressed);

	int Check(const unsigned char *vch);

	bool ECC_InitSanityCheck();
	// init secp256
	void ECC_Start();
	// close secp256
	void ECC_Stop();

	// context for verify
	void ECC_Verify_Start();
	void ECC_Verify_End();

	//get context for sign
	secp256k1_context* GetSignContext() const;
	//get context for verify
	secp256k1_context* GetVerifyContext() const;

	int ecdsa_signature_parse_der_lax(const secp256k1_context* ctx, secp256k1_ecdsa_signature* sig, const unsigned char *input, size_t inputlen);

protected:
	
private:

	secp256k1_context*		secp256k1_context_sign_;
	secp256k1_context*		secp256k1_context_verify_;
};

inline secp256k1_context* Secp256Context::GetSignContext() const
{
	return secp256k1_context_sign_;
}

inline secp256k1_context* Secp256Context::GetVerifyContext() const
{
	return secp256k1_context_verify_;
}

}