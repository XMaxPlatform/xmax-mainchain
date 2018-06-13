
namespace pro
{
	template <class T>
	class singleton
	{
		singleton<T>(const singleton<T>&) = delete;
		singleton<T>& operator=(const singleton<T>&) = delete;

	protected:
		singleton<T>() = default;
		virtual ~singleton<T>() = default;

	public:
		static inline T& getInstance() {
			static T inst;
			return inst;
		}			

	};

#define DECLARE_USE_SINGLETON(CLS) \
		CLS();\
		virtual ~CLS(); \
		friend class pro::singleton<CLS>;
}