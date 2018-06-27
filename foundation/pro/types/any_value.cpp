/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/types/any_value.hpp>

namespace pro
{
	AnyValue::AnyValue()
		: code_(AnyType::Type_Void)
	{
		val_.anyval = 0;
	}

	AnyValue::AnyValue(string&& v)
	{
		assign(std::forward<string>(v));
	}

	AnyValue::AnyValue(DataStream&& v)
	{
		assign(std::forward<DataStream>(v));
	}
	AnyValue::AnyValue(const void* data, size_t len)
	{
		assign(data, len);
	}

	AnyValue::AnyValue(AnyValue&& v)
	{
		assign(std::forward<AnyValue>(v));
	}

	AnyValue::~AnyValue()
	{
		clearImpl();
	}

	AnyValue & AnyValue::operator = (AnyValue&& v)
	{
		clearImpl();
		assign(std::forward<AnyValue>(v));
		return *this;
	}


	void AnyValue::Clear()
	{
		clearImpl();
		code_ = AnyType::Type_Void;
	}
	void AnyValue::assign(const string& v)
	{
		newContainer<AnyType::FString, AnyType::Type_String>() = v;
	}
	void AnyValue::assign(string&& v)
	{
		newContainer<AnyType::FString, AnyType::Type_String>() = std::forward<string>(v);
	}
	void AnyValue::assign(const char* v)
	{
		newContainer<AnyType::FString, AnyType::Type_String>() = v;
	}

	void AnyValue::assign(const DataStream& v)
	{
		newContainer<AnyType::FStream, AnyType::Type_Stream>() = v;
	}

	void AnyValue::assign(DataStream&& v)
	{
		newContainer<AnyType::FStream, AnyType::Type_Stream>() = std::forward<DataStream>(v);
	}

	void AnyValue::assign(const void* data, size_t len)
	{
		auto& stream = newContainer<AnyType::FStream, AnyType::Type_Stream>();
		stream.data.resize(len);
		memcpy(&stream.data[0], data, len);
	}

	void AnyValue::assign(const AnyValue& v)
	{
		if (AnyType::IsSimpleType(v.code_))
		{
			memcpy(this, &v, sizeof(v));
		}
		else
		{
			setContainerImpl(v.code_, v.val_.anyptr->CopySelf());
		}
	}

	void AnyValue::assign(AnyValue&& v)
	{
		if (AnyType::IsSimpleType(v.code_))
		{
			memcpy(this, &v, sizeof(v));
		}
		else
		{
			setContainerImpl(code_, v.val_.anyptr);
		}
	}

	void AnyValue::clearImpl()
	{
		if (!AnyType::IsSimpleType(code_))
		{
			IAnyContainer::Delete(val_.anyptr);
			val_.anyptr = nullptr;
		}
	}
}