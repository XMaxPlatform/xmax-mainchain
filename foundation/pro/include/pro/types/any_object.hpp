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
			AnyValue value;
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
		
		// get value by key, if not found, push a  default Entity of key.
		const AnyValue& At(const EntityKey& key) const;
		AnyValue& At(const EntityKey& key);

		void Set(const EntityKey& key, const AnyValue& val);
		void Emplace(EntityKey&& key, AnyValue&& val);
		Iterator Erase(ConstIterator it);

		bool Remove(const EntityKey& key);

		void Clear();

		// if key found and the value of key is valid, return true, otherwise return false.
		bool IsValid(const EntityKey& key) const;

		// the some as fuction At()
		const AnyValue& operator [] (const EntityKey& key) const;
		AnyValue& operator [] (const EntityKey& key);

		const AnyValue& operator [] (size_t index) const;
		AnyValue& operator [] (size_t index);

		ConstIterator Begin() const;

		ConstIterator End() const;

		// get Entity by index. if not found, throw
		const Entity& GetEntity(size_t index) const;
		size_t Count() const;
	public: // for std.
		ConstIterator begin() const;

		ConstIterator end() const;

	private:
		Iterator Begin();	
		Iterator End();
		Iterator begin();
		Iterator end();
		Iterator find(const EntityKey& key);
		ConstIterator find(const EntityKey& key) const;

		AnyValue& emplaceback(EntityKey&& key) const;
		AnyValue& pushback(const EntityKey& key) const;

		mutable EntityContainer entities_;

		static const AnyValue EmptyValue;
	};

}