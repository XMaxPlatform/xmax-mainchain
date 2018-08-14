#pragma once

#include "pro/utils/singleton.hpp"

#include <secp256k1.h>
#include <secp256k1_recovery.h>

namespace pro
{
	class Secp256Context : public Singleton<Secp256Context>
{
public:

	int EC_PrivateKey_Import(const secp256k1_context* ctx, unsigned char *out32, const unsigned char *privkey, size_t privkeylen);

	int EC_PrivateKey_Export(const secp256k1_context* ctx, unsigned char* privkey, size_t* privkeylen, const unsigned char* key32, int compressed);

	int Check(const unsigned char *vch);

	bool ECC_InitSanityCheck();

	void ECC_Start();

	void ECC_Stop();

	secp256k1_context* GetSecpContext() const;

protected:
	
private:

	secp256k1_context*		secp256k1_context_sign_;
};

inline secp256k1_context* Secp256Context::GetSecpContext() const
{
	return secp256k1_context_sign_;
}

}