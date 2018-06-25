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
	AnyVaule::AnyVaule(const void* data, size_t len)
	{
		assign(data, len);
	}
	AnyVaule::AnyVaule(const DataStream& v)
	{
		assign(v);
	}


	AnyVaule::AnyVaule(const AnyVaule& v)
	{
		assign(v);
	}
	AnyVaule::AnyVaule(AnyVaule&& v)
	{
		assign(std::move(v));
	}

	AnyVaule::~AnyVaule()
	{
		clearImpl();
	}

	AnyVaule & AnyVaule::operator = (const AnyVaule& v)
	{
		assign(v);
		return *this;
	}

	AnyVaule & AnyVaule::operator = (AnyVaule&& v)
	{
		assign(std::move(v));
		return *this;
	}


	void AnyVaule::Clear()
	{
		clearImpl();
		code_ = Type_Void;
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
		newType<string>(Type_String) = std::forward<string>(v);
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
		newType<DataStream>(Type_Stream) = std::forward<DataStream>(v);
	}

	void AnyVaule::assign(const void* data, size_t len)
	{
		auto& stream = newType<DataStream>(Type_Stream);
		stream.data.resize(len);
		memcpy(&stream.data[0], data, len);
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

	void AnyVaule::assign(AnyVaule&& v)
	{
		switch (v.GetType())
		{
		case AnyVaule::Type_String:
		{
			assign(std::forward<std::string>(*v.val_.str));
			break;
		}
		case AnyVaule::Type_Stream:
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