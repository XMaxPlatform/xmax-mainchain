/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/types/any_value.hpp>

namespace pro
{
	AnyVaule::AnyVaule()
		: code_(TypeCode::Type_Void)
	{

	}
	AnyVaule::AnyVaule(bool v)
	{
		assign(v);
	}
	AnyVaule::AnyVaule(int32_t v)
	{
		assign(v);
	}
	AnyVaule::AnyVaule(uint32_t v)
	{
		assign(v);
	}
	AnyVaule::AnyVaule(int64_t v)
	{
		assign(v);
	}
	AnyVaule::AnyVaule(uint64_t v)
	{
		assign(v);
	}
	AnyVaule::AnyVaule(double v)
	{
		assign(v);
	}
	AnyVaule::AnyVaule(const char* v)
	{
		assign(v);
	}
	AnyVaule::AnyVaule(string&& v)
	{
		assign(std::forward<string>(v));
	}
	AnyVaule::AnyVaule(const string& v)
	{
		assign(v);
	}
	AnyVaule::AnyVaule(DataStream&& v)
	{
		assign(std::forward<DataStream>(v));
	}
	AnyVaule::AnyVaule(const DataStream& v)
	{
		assign(v);
	}


	AnyVaule::AnyVaule(const AnyVaule& v)
	{

	}

	AnyVaule::~AnyVaule()
	{
		clearImpl();
	}
	void AnyVaule::assign(bool v)
	{
		setValue(v, Type_Bool);
	}
	void AnyVaule::assign(int32_t v)
	{
		setValue(v, Type_I32);
	}
	void AnyVaule::assign(uint32_t v)
	{
		setValue(v, Type_UI32);
	}
	void AnyVaule::assign(int64_t v)
	{
		setValue(v, Type_I64);
	}
	void AnyVaule::assign(uint64_t v)
	{
		setValue(v, Type_UI64);
	}
	void AnyVaule::assign(double v)
	{
		setValue(v, Type_F64);
	}
	void AnyVaule::assign(const string& v)
	{

		newType<string>(Type_String) = v;
	}
	void AnyVaule::assign(string&& v)
	{
		newType<string>(Type_String) = v;
	}
	void AnyVaule::assign(const char* v)
	{
		newType<string>(Type_String) = v;
	}

	void AnyVaule::assign(const DataStream& v)
	{
		newType<DataStream>(Type_Stream) = v;
	}

	void AnyVaule::assign(DataStream&& v)
	{
		newType<DataStream>(Type_Stream) = v;
	}

	void AnyVaule::assign(const AnyVaule& v)
	{
		switch (v.GetType())
		{
		case AnyVaule::Type_String:
		{
			assign(*v.val_.str);
			break;
		}
		case AnyVaule::Type_Stream:
		{
			assign(*v.val_.stream);
			break;
		}
		default:
		{
			memcpy(this, &v, sizeof(v));
		}
			break;
		}
	}

	void AnyVaule::Clear()
	{
		clearImpl();
		code_ = Type_Void;
	}

	void AnyVaule::clearImpl()
	{
		switch (code_)
		{
		case AnyVaule::Type_String:
		{
			delete val_.str;
			val_.str = nullptr;
		}
		break;
		case AnyVaule::Type_Stream:
		{
			delete val_.stream;
			val_.stream = nullptr;
		}
		break;
		default:
			break;
		}
	}
}