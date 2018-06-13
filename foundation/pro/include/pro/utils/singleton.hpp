#pragma once

namespace pro
{
	template <class T>
	class Singleton
	{
		Singleton<T>(const Singleton<T>&) = delete;
		Singleton<T>& operator=(const Singleton<T>&) = delete;

	protected:
		Singleton<T>() = default;
		virtual ~Singleton<T>() = default;

	public:
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