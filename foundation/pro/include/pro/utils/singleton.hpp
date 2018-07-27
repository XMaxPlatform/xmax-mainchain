#pragma once

namespace pro
{
	/**
	* Template class of Singleton
	*/
	template <class T>
	class Singleton {
	private:
		Singleton<T>(const Singleton<T>&) = delete;
		Singleton<T>& operator=(const Singleton<T>&) = delete;

		/**
		* can not new a concrete singleton, use GetInstantce instead
		*/
	void* operator new(size_t nSize)
	{
		return malloc(nSize);
	}

	protected:
		Singleton<T>() = default;
		virtual ~Singleton<T>() = default;

	public:		
		/**
		* Get The singleton instance.
		*/
		static inline T& GetInstance() {
			static T inst;
			return inst;
		}			

	};

#define DECLARE_USE_SINGLETON(CLS) \
		CLS();\
		virtual ~CLS(); \
		friend class pro::Singleton<CLS>;
}