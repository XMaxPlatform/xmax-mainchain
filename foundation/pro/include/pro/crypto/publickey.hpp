#pragma once

#include "pro/utils/sha256.hpp"

namespace pro
{
	class CSHA256;
	class PublicKey
	{
	public:

		PublicKey()
		{
			_Invalidate();
		}

		template <typename T>
		PublicKey(const T pbegin, const T pend)
		{
			Set(pbegin, pend);
		}

		PublicKey(const std::vector<unsigned char>& _vch)
		{
			Set(_vch.begin(), _vch.end());
		}

		template <typename T>
		void Set(const T pbegin, const T pend)
		{
			int len = pend == pbegin ? 0 : _GetLen(pbegin[0]);
			if (len && len == (pend - pbegin))
				memcpy(vch_, (unsigned char*)&pbegin[0], len);
			else
				_Invalidate();
		}

		unsigned int Size() const;
		const unsigned char* Begin() const;
		const unsigned char* End() const;
		const unsigned char& operator[](unsigned int pos) const 
		{ 
			return vch_[pos]; 
		}

		//! Comparator implementation.
		friend bool operator==(const PublicKey& a, const PublicKey& b)
		{
			return a.vch_[0] == b.vch_[0] &&
				memcmp(a.vch_, b.vch_, a.Size()) == 0;
		}

		friend bool operator!=(const PublicKey& a, const PublicKey& b)
		{
			return !(a == b);
		}

		friend bool operator<(const PublicKey& a, const PublicKey& b)
		{
			return a.vch_[0] < b.vch_[0] ||
				(a.vch_[0] == b.vch_[0] && memcmp(a.vch_, b.vch_, a.Size()) < 0);
		}

		CSHA256 GetPublicKeyHash() const
		{
			DoubleSHA256 ds;
			std::vector<unsigned char> vec;
			vec.assign(vch_, vch_ + Size());
			ds.Hash(vec.begin(), vec.end());
			return ds.GetSHA();
		}

		bool IsValid() const
		{
			return Size() > 0;
		}

		//! fully validate whether this is a valid public key (more expensive than IsValid())
		bool IsFullyValid() const;

		//! Check whether this is a compressed public key.
		bool IsCompressed() const
		{
			return Size() == 33;
		}

		/**
		* Verify a DER signature (~72 bytes).
		* If this public key is not fully valid, the return value will be false.
		*/
		bool Verify(const CSHA256& hash, const std::vector<unsigned char>& vchSig) const;

		/**
		* Check whether a signature is normalized (lower-S).
		*/
		static bool CheckLowS(const std::vector<unsigned char>& vchSig);

		//! Recover a public key from a compact signature.
		bool RecoverCompact(const CSHA256& hash, const std::vector<unsigned char>& vchSig);

		//! Turn this public key into an uncompressed public key.
		bool Decompress();

	protected:

		unsigned int static _GetLen(unsigned char chHeader);

		void _Invalidate();

	private:

		/**
		* Just store the serialized data.
		* Its length can very cheaply be computed from the first byte.
		*/
		unsigned char vch_[65];
	};

	inline unsigned int PublicKey::_GetLen(unsigned char chHeader)
	{
		if (chHeader == 2 || chHeader == 3)
			return 33;
		if (chHeader == 4 || chHeader == 6 || chHeader == 7)
			return 65;
		return 0;
	}

	inline void PublicKey::_Invalidate()
	{
		vch_[0] = 0xFF;
	}

	inline unsigned int PublicKey::Size() const
	{
		return _GetLen(vch_[0]);
	}

	inline const unsigned char* PublicKey::Begin() const
	{
		return vch_;
	}

	inline const unsigned char* PublicKey::End() const
	{
		return vch_ + Size();
	}
}