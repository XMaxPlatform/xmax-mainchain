/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/build.hpp>
#include <pro/types/any_value.hpp>
#include <vector>
#include <memory>

namespace pro
{
	class AnyObject
	{
	public:
		AnyObject() = default;

		~AnyObject();
		typedef string EntityKey;
		struct Entity
		{
			EntityKey key;
			AnyValue entity;
			Entity() = default;
			Entity(const EntityKey& k)
				: key(k)
			{
			}
			Entity(EntityKey&& k)
				: key(std::forward<EntityKey>(k))
			{
			}

		};
		typedef std::vector<AnyObject::Entity> EntityContainer;
		typedef EntityContainer::iterator Iterator;
		typedef EntityContainer::const_iterator ConstIterator;

		
		const AnyValue& At(const string& key) const;

		AnyValue& At(const string& key);

		void Set(string&& key, AnyValue&& val);

		void Clear();


		const AnyValue& operator [] (const string& key) const
		{
			return At(key);
		}
		AnyValue& operator [] (const string& key)
		{
			return At(key);
		}


	private:

		Iterator find(const string& key)
		{
			for (auto it = entities_.begin(); it != entities_.end(); ++it)
			{
				if (it->key == key)
				{
					return it;
				}
			}
			return entities_.end();
		}
		ConstIterator find(const string& key) const
		{
			for (auto it = entities_.begin(); it != entities_.end(); ++it)
			{
				if (it->key == key)
				{
					return it;
				}
			}
			return entities_.end();
		}

		AnyValue& emplaceback(string&& key) const
		{
			return entities_.emplace_back(Entity(key)).entity;
		}
		AnyValue& emplaceback(const string& key) const
		{
			return entities_.emplace_back(Entity(key)).entity;
		}

		mutable EntityContainer entities_;
	};

}