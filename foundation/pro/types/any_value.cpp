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
	AnyVaule::~AnyVaule()
	{
		Clear();
	}

	void AnyVaule::SetValue(int32_t v)
	{
		Clear();
		setValue(v);
		setCode(Type_I32);
	}
	void AnyVaule::SetValue(uint32_t v)
	{
		Clear();
		setValue(v);
		setCode(Type_UI32);
	}
	void AnyVaule::SetValue(int64_t v)
	{
		Clear();
		setValue(v);
		setCode(Type_I64);
	}
	void AnyVaule::SetValue(uint64_t v)
	{
		Clear();
		setValue(v);
		setCode(Type_UI64);
	}
	void AnyVaule::SetValue(double v)
	{
		Clear();
		setValue(v);
		setCode(Type_F64);
	}
	void AnyVaule::SetValue(const string& v)
	{
		Clear();
		val_.str = new string;

		*val_.str = v;
		setCode(Type_String);
	}
	void AnyVaule::SetValue(const char* v)
	{
		Clear();
		val_.str = new string;

		*val_.str = v;
		setCode(Type_String);
	}

	void AnyVaule::Clear()
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