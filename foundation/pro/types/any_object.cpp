/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/types/any_object.hpp>

namespace pro
{


	AnyObject::~AnyObject()
	{

	}

	void AnyObject::Clear()
	{
		entities_.clear();
	}

	const AnyValue& AnyObject::At(const string& key) const
	{
		auto it = find(key);
		if (it != entities_.end())
		{
			return it->entity;
		}
		return emplaceback(key);
	}

	AnyValue& AnyObject::At(const string& key)
	{
		auto it = find(key);
		if (it != entities_.end())
		{
			return it->entity;
		}
		return emplaceback(key);
	}

	void AnyObject::Set(string&& key, AnyValue&& val)
	{
		auto it = find(key);
		if (it != entities_.end())
		{
			it->entity = val;
		}
		else
		{
			emplaceback(std::forward<string>(key)) = std::forward<AnyValue>(val);
		}
		
	}

}