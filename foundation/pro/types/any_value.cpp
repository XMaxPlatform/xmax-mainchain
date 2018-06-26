/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/types/any_value.hpp>

namespace pro
{
	AnyValue::AnyValue()
		: code_(TypeCode::Type_Void)
	{

	}
	AnyValue::AnyValue(bool v)
	{
		assign(v);
	}
	AnyValue::AnyValue(int32_t v)
	{
		assign(v);
	}
	AnyValue::AnyValue(uint32_t v)
	{
		assign(v);
	}
	AnyValue::AnyValue(int64_t v)
	{
		assign(v);
	}
	AnyValue::AnyValue(uint64_t v)
	{
		assign(v);
	}
	AnyValue::AnyValue(double v)
	{
		assign(v);
	}
	AnyValue::AnyValue(const char* v)
	{
		assign(v);
	}
	AnyValue::AnyValue(string&& v)
	{
		assign(std::forward<string>(v));
	}
	AnyValue::AnyValue(const string& v)
	{
		assign(v);
	}
	AnyValue::AnyValue(DataStream&& v)
	{
		assign(std::forward<DataStream>(v));
	}
	AnyValue::AnyValue(const void* data, size_t len)
	{
		assign(data, len);
	}
	AnyValue::AnyValue(const DataStream& v)
	{
		assign(v);
	}


	AnyValue::AnyValue(const AnyValue& v)
	{
		assign(v);
	}
	AnyValue::AnyValue(AnyValue&& v)
	{
		assign(std::move(v));
	}

	AnyValue::~AnyValue()
	{
		clearImpl();
	}

	AnyValue & AnyValue::operator = (const AnyValue& v)
	{
		assign(v);
		return *this;
	}

	AnyValue & AnyValue::operator = (AnyValue&& v)
	{
		assign(std::move(v));
		return *this;
	}


	void AnyValue::Clear()
	{
		clearImpl();
		code_ = Type_Void;
	}

	void AnyValue::assign(bool v)
	{
		setValue(v, Type_Bool);
	}
	void AnyValue::assign(int32_t v)
	{
		setValue(v, Type_I32);
	}
	void AnyValue::assign(uint32_t v)
	{
		setValue(v, Type_UI32);
	}
	void AnyValue::assign(int64_t v)
	{
		setValue(v, Type_I64);
	}
	void AnyValue::assign(uint64_t v)
	{
		setValue(v, Type_UI64);
	}
	void AnyValue::assign(double v)
	{
		setValue(v, Type_F64);
	}
	void AnyValue::assign(const string& v)
	{
		newType<string>(Type_String) = v;
	}
	void AnyValue::assign(string&& v)
	{
		newType<string>(Type_String) = std::forward<string>(v);
	}
	void AnyValue::assign(const char* v)
	{
		newType<string>(Type_String) = v;
	}

	void AnyValue::assign(const DataStream& v)
	{
		newType<DataStream>(Type_Stream) = v;
	}

	void AnyValue::assign(DataStream&& v)
	{
		newType<DataStream>(Type_Stream) = std::forward<DataStream>(v);
	}

	void AnyValue::assign(const void* data, size_t len)
	{
		auto& stream = newType<DataStream>(Type_Stream);
		stream.data.resize(len);
		memcpy(&stream.data[0], data, len);
	}

	void AnyValue::assign(const AnyValue& v)
	{
		switch (v.GetType())
		{
		case AnyValue::Type_String:
		{
			assign(*v.val_.str);
			break;
		}
		case AnyValue::Type_Stream:
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

	void AnyValue::assign(AnyValue&& v)
	{
		switch (v.GetType())
		{
		case AnyValue::Type_String:
		{
			assign(std::forward<std::string>(*v.val_.str));
			break;
		}
		case AnyValue::Type_Stream:
		{
			assign(std::forward<DataStream>(*v.val_.stream));
			break;
		}
		default:
		{
			memcpy(this, &v, sizeof(v));
		}
		break;
		}
	}

	void AnyValue::clearImpl()
	{
		switch (code_)
		{
		case AnyValue::Type_String:
		{
			delete asPtr<string>();
			val_.anyptr = nullptr;
		}
		break;
		case AnyValue::Type_Stream:
		{
			delete asPtr<DataStream>();
			val_.anyptr = nullptr;
		}
		break;
		default:
			break;
		}
	}
}