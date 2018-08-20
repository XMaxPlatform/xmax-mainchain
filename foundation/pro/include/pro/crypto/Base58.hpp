#pragma once

#include <string>
#include <vector>

namespace pro
{
	/**
	* Encode a byte sequence as a base58-encoded string.
	* pbegin and pend cannot be NULL, unless both are.
	*/
	std::string EncodeBase58(const unsigned char* pbegin, const unsigned char* pend);

	/**
	* Encode a byte vector as a base58-encoded string
	*/
	std::string EncodeBase58(const std::vector<unsigned char>& vch);

	/**
	* Decode a base58-encoded string (psz) into a byte vector (vchRet).
	* return true if decoding is successful.
	* psz cannot be NULL.
	*/
	bool DecodeBase58(const char* psz, std::vector<unsigned char>& vchRet);

	/**
	* Decode a base58-encoded string (str) into a byte vector (vchRet).
	* return true if decoding is successful.
	*/
	bool DecodeBase58(const std::string& str, std::vector<unsigned char>& vchRet);

	/**
	* Encode a byte vector into a base58-encoded string, including checksum
	*/
	std::string EncodeBase58Check(const std::vector<unsigned char>& vchIn);

	/**
	* Decode a base58-encoded string (psz) that includes a checksum into a byte
	* vector (vchRet), return true if decoding is successful
	*/
	inline bool DecodeBase58Check(const char* psz, std::vector<unsigned char>& vchRet);

	/**
	* Decode a base58-encoded string (str) that includes a checksum into a byte
	* vector (vchRet), return true if decoding is successful
	*/
	inline bool DecodeBase58Check(const std::string& str, std::vector<unsigned char>& vchRet);

	class PrivateKey;
	class Base58String;
	class Base58Data
	{
	public:

		void SetData(const std::vector<unsigned char> &vchVersionIn, const void* pdata, size_t nSize);
		void SetData(const std::vector<unsigned char> &vchVersionIn, const unsigned char *pbegin, const unsigned char *pend);

		bool SetStringWithVersionBytes(const char* psz, unsigned int nVersionBytes);
		bool SetString(const char* psz) {
			return SetStringWithVersionBytes(psz, 1);
		}
		bool SetString(const std::string& str);
		bool SetBase58string(const Base58String& str);

		std::string ToString() const;
		int CompareTo(const Base58Data& b58) const;

		bool operator==(const Base58Data& b58) const { return CompareTo(b58) == 0; }
		bool operator<=(const Base58Data& b58) const { return CompareTo(b58) <= 0; }
		bool operator>=(const Base58Data& b58) const { return CompareTo(b58) >= 0; }
		bool operator< (const Base58Data& b58) const { return CompareTo(b58) < 0; }
		bool operator> (const Base58Data& b58) const { return CompareTo(b58) > 0; }
		friend class PrivateKey;
	protected:


	private:

		std::vector<unsigned char>  vchVersion_;

		std::vector<unsigned char>	vchData_;
	};
}