/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <memory>
#include <pro/types/generictypes.hpp>
#include <pro/types/any_object.hpp>

namespace pro
{

	class ArgsPack : private AnyObject
	{
	public:
		ArgsPack()
		{

		}

		ArgsPack& operator()(const AnyObject::EntityKey& key, const AnyValue& val)
		{
			Append(key, val);
			return *this;
		}
		ArgsPack& operator()(AnyObject::EntityKey&& key, AnyValue&& val)
		{
			Append(std::forward<EntityKey>(key), std::forward<AnyValue>(val));
			return *this;
		}
		ArgsPack& operator()()
		{
			return *this;
		}
		template<typename T>
		ArgsPack& operator()(const AnyObject::EntityKey& key, const T& val)
		{
			Append(key, val);
			return *this;
		}

		template<typename T>
		ArgsPack& operator()(AnyObject::EntityKey&& key, T&& val)
		{
			Append(std::forward<EntityKey>(key), 
				AnyValue(std::forward<T>(val))
			);
			return *this;
		}

		const AnyObject& GetArgs() const
		{
			return *static_cast<const AnyObject*>(this);
		}

	protected:
		void Append(const EntityKey& key, const AnyValue& val)
		{
			pushback(key) = val;
		}
		void Append(EntityKey&& key, AnyValue&& val)
		{
			pushback(std::forward<EntityKey>(key)) = std::forward<AnyValue>(val);
		}

	};
}


