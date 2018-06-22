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
	AnyVaule::AnyVaule(const string& v)
	{
		assign(v);
	}
	AnyVaule::AnyVaule(const char* v)
	{
		assign(v);
	}


	AnyVaule::~AnyVaule()
	{
		clearImpl();
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
		val_.str = new string;

		*val_.str = v;
		setCode(Type_String);
	}
	void AnyVaule::assign(const char* v)
	{
		val_.str = new string;

		*val_.str = v;
		setCode(Type_String);
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
		case pro::AnyVaule::Type_String:
		{
			delete val_.str;
			val_.str = nullptr;
		}
		break;
		default:
			break;
		}
	}
}