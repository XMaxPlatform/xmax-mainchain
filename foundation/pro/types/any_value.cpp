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

	void AnyVaule::SetValue(int32_t v)
	{
		setValue(v);
		setCode(Type_I32);
	}
	void AnyVaule::SetValue(uint32_t v)
	{
		setValue(v);
		setCode(Type_UI32);
	}
	void AnyVaule::SetValue(int64_t v)
	{
		setValue(v);
		setCode(Type_I64);
	}
	void AnyVaule::SetValue(uint64_t v)
	{
		setValue(v);
		setCode(Type_UI64);
	}
	void AnyVaule::SetValue(double v)
	{
		setValue(v);
		setCode(Type_F64);
	}
	void AnyVaule::SetValue(const string& v)
	{
		val_.str = new string;

		*val_.str = v;
		setCode(Type_String);
	}
	void AnyVaule::SetValue(const char* v)
	{
		val_.str = new string;

		*val_.str = v;
		setCode(Type_String);
	}
}