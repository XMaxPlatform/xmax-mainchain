#include "pro/crypto/secp256_util.hpp"
#include "pro/log/log.hpp"
#include "pro/crypto/privatekey.hpp"
#include <vector>
#include <memory>

namespace pro
{
int Secp256Context::EC_PrivateKey_Import(const secp256k1_context* ctx, unsigned char *out32, const unsigned char *privkey, size_t privkeylen)
{
	const unsigned char *end = privkey + privkeylen;
	int lenb = 0;
	int len = 0;
	memset(out32, 0, 32);
	/* sequence header */
	if (end < privkey + 1 || *privkey != 0x30) 
	{
		return 0;
	}
	privkey++;
	/* sequence length constructor */
	if (end < privkey + 1 || !(*privkey & 0x80)) 
	{
		return 0;
	}
	lenb = *privkey & ~0x80; privkey++;
	if (lenb < 1 || lenb > 2) 
	{
		return 0;
	}
	if (end < privkey + lenb) 
	{
		return 0;
	}
	/* sequence length */
	len = privkey[lenb - 1] | (lenb > 1 ? privkey[lenb - 2] << 8 : 0);
	privkey += lenb;
	if (end < privkey + len)
	{
		return 0;
	}
	/* sequence element 0: version number (=1) */
	if (end < privkey + 3 || privkey[0] != 0x02 || privkey[1] != 0x01 || privkey[2] != 0x01) 
	{
		return 0;
	}
	privkey += 3;
	/* sequence element 1: octet string, up to 32 bytes */
	if (end < privkey + 2 || privkey[0] != 0x04 || privkey[1] > 0x20 || end < privkey + 2 + privkey[1]) 
	{
		return 0;
	}
	memcpy(out32 + 32 - privkey[1], privkey + 2, privkey[1]);
	if (!secp256k1_ec_seckey_verify(ctx, out32)) 
	{
		memset(out32, 0, 32);
		return 0;
	}
	return 1;
}

int Secp256Context::EC_PrivateKey_Export(const secp256k1_context* ctx, unsigned char* privkey, size_t* privkeylen, const unsigned char* key32, int compressed)
{
	secp256k1_pubkey pubkey;
	size_t pubkeylen = 0;
	if (!secp256k1_ec_pubkey_create(ctx, &pubkey, key32)) 
	{
		*privkeylen = 0;
		return 0;
	}

	if (compressed) 
	{
		static const unsigned char begin[] = 
		{
			0x30,0x81,0xD3,0x02,0x01,0x01,0x04,0x20
		};
		static const unsigned char middle[] = 
		{
			0xA0,0x81,0x85,0x30,0x81,0x82,0x02,0x01,0x01,0x30,0x2C,0x06,0x07,0x2A,0x86,0x48,
			0xCE,0x3D,0x01,0x01,0x02,0x21,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
			0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
			0xFF,0xFF,0xFE,0xFF,0xFF,0xFC,0x2F,0x30,0x06,0x04,0x01,0x00,0x04,0x01,0x07,0x04,
			0x21,0x02,0x79,0xBE,0x66,0x7E,0xF9,0xDC,0xBB,0xAC,0x55,0xA0,0x62,0x95,0xCE,0x87,
			0x0B,0x07,0x02,0x9B,0xFC,0xDB,0x2D,0xCE,0x28,0xD9,0x59,0xF2,0x81,0x5B,0x16,0xF8,
			0x17,0x98,0x02,0x21,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
			0xFF,0xFF,0xFF,0xFF,0xFE,0xBA,0xAE,0xDC,0xE6,0xAF,0x48,0xA0,0x3B,0xBF,0xD2,0x5E,
			0x8C,0xD0,0x36,0x41,0x41,0x02,0x01,0x01,0xA1,0x24,0x03,0x22,0x00
		};
		unsigned char *ptr = privkey;
		memcpy(ptr, begin, sizeof(begin)); 
		ptr += sizeof(begin);
		memcpy(ptr, key32, 32); 
		ptr += 32;
		memcpy(ptr, middle, sizeof(middle)); 
		ptr += sizeof(middle);
		pubkeylen = 33;
		secp256k1_ec_pubkey_serialize(ctx, ptr, &pubkeylen, &pubkey, SECP256K1_EC_COMPRESSED);
		ptr += pubkeylen;
		*privkeylen = ptr - privkey;
	}
	else 
	{
		// NOTE: Actually NOT USED.
		Errorf("");
	}
	return 1;
}

int Secp256Context::Check(const unsigned char *vch)
{
	return secp256k1_ec_seckey_verify(secp256k1_context_sign_, vch);
}

bool Secp256Context::ECC_InitSanityCheck()
{
	PrivateKey key;
	key.MakeNewKey(true);
	PublicKey pubkey = key.GetPubKey();
	return key.VerifyPublicKey(pubkey);
}

void Secp256Context::ECC_Start()
{
	secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
	if (ctx != nullptr)
	{
		std::vector<unsigned char> vseed(32);
		//GetRandBytes(vseed.data(), 32);
		bool ret = secp256k1_context_randomize(ctx, vseed.data());
		if (!ret)
		{
			Errorf("fail to update the context randomization");
		}
		secp256k1_context_sign_ = ctx;
	}
}

void Secp256Context::ECC_Verify_Start()
{
	secp256k1_context_verify_ = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
}

void Secp256Context::ECC_Stop()
{
	secp256k1_context *ctx  = secp256k1_context_sign_;
	secp256k1_context_sign_ = NULL;

	if (ctx) 
	{
		secp256k1_context_destroy(ctx);
	}
}

void Secp256Context::ECC_Verify_End()
{
	secp256k1_context_destroy(secp256k1_context_verify_);
	secp256k1_context_verify_ = NULL;
}

int Secp256Context::ecdsa_signature_parse_der_lax(const secp256k1_context* ctx, secp256k1_ecdsa_signature* sig, const unsigned char *input, size_t inputlen)
{
	size_t rpos, rlen, spos, slen;
	size_t pos = 0;
	size_t lenbyte;
	unsigned char tmpsig[64] = { 0 };
	int overflow = 0;

	/* Hack to initialize sig with a correctly-parsed but invalid signature. */
	secp256k1_ecdsa_signature_parse_compact(ctx, sig, tmpsig);

	/* Sequence tag byte */
	if (pos == inputlen || input[pos] != 0x30) {
		return 0;
	}
	pos++;

	/* Sequence length bytes */
	if (pos == inputlen) {
		return 0;
	}
	lenbyte = input[pos++];
	if (lenbyte & 0x80) {
		lenbyte -= 0x80;
		if (pos + lenbyte > inputlen) {
			return 0;
		}
		pos += lenbyte;
	}

	/* Integer tag byte for R */
	if (pos == inputlen || input[pos] != 0x02) {
		return 0;
	}
	pos++;

	/* Integer length for R */
	if (pos == inputlen) {
		return 0;
	}
	lenbyte = input[pos++];
	if (lenbyte & 0x80) {
		lenbyte -= 0x80;
		if (pos + lenbyte > inputlen) {
			return 0;
		}
		while (lenbyte > 0 && input[pos] == 0) {
			pos++;
			lenbyte--;
		}
		if (lenbyte >= sizeof(size_t)) {
			return 0;
		}
		rlen = 0;
		while (lenbyte > 0) {
			rlen = (rlen << 8) + input[pos];
			pos++;
			lenbyte--;
		}
	}
	else {
		rlen = lenbyte;
	}
	if (rlen > inputlen - pos) {
		return 0;
	}
	rpos = pos;
	pos += rlen;

	/* Integer tag byte for S */
	if (pos == inputlen || input[pos] != 0x02) {
		return 0;
	}
	pos++;

	/* Integer length for S */
	if (pos == inputlen) {
		return 0;
	}
	lenbyte = input[pos++];
	if (lenbyte & 0x80) {
		lenbyte -= 0x80;
		if (pos + lenbyte > inputlen) {
			return 0;
		}
		while (lenbyte > 0 && input[pos] == 0) {
			pos++;
			lenbyte--;
		}
		if (lenbyte >= sizeof(size_t)) {
			return 0;
		}
		slen = 0;
		while (lenbyte > 0) {
			slen = (slen << 8) + input[pos];
			pos++;
			lenbyte--;
		}
	}
	else {
		slen = lenbyte;
	}
	if (slen > inputlen - pos) {
		return 0;
	}
	spos = pos;
	pos += slen;

	/* Ignore leading zeroes in R */
	while (rlen > 0 && input[rpos] == 0) {
		rlen--;
		rpos++;
	}
	/* Copy R value */
	if (rlen > 32) 
	{
		overflow = 1;
	}
	else 
	{
		memcpy(tmpsig + 32 - rlen, input + rpos, rlen);
	}

	/* Ignore leading zeroes in S */
	while (slen > 0 && input[spos] == 0) 
	{
		slen--;
		spos++;
	}
	/* Copy S value */
	if (slen > 32) 
	{
		overflow = 1;
	}
	else 
	{
		memcpy(tmpsig + 64 - slen, input + spos, slen);
	}

	if (!overflow) 
	{
		overflow = !secp256k1_ecdsa_signature_parse_compact(ctx, sig, tmpsig);
	}
	if (overflow) 
	{
		/* Overwrite the result again with a correctly-parsed but invalid
		signature if parsing failed. */
		memset(tmpsig, 0, 64);
		secp256k1_ecdsa_signature_parse_compact(ctx, sig, tmpsig);
	}
	return 1;
}

}