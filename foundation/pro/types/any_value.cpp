/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/types/any_value.hpp>

namespace pro
{
	AnyVaule::AnyVaule()
		: code(Type_Void)
	{

	}


	void AnyVaule::Set(int32_t v)
	{
		setValue(v);
		setCode(Type_I32);
	}
	void AnyVaule::Set(uint32_t v)
	{
		setValue(v);
		setCode(Type_UI32);
	}
	void AnyVaule::Set(int64_t v)
	{
		setValue(v);
		setCode(Type_I64);
	}
	void AnyVaule::Set(uint64_t v)
	{
		setValue(v);
		setCode(Type_UI64);
	}
	void AnyVaule::Set(double v)
	{
		setValue(v);
		setCode(Type_F64);
	}
}