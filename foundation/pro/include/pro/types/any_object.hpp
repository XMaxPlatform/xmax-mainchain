/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/build.hpp>

#include <pro/types/any_value.hpp>

namespace pro
{
	class AnyObject
	{
	public:
		AnyObject() = default;
		AnyObject(const AnyObject&) = delete;
		AnyObject& operator = (const AnyObject &) = delete;
		~AnyObject();
		typedef string EntityKey;
		struct EntityPair
		{
			EntityKey key;
			AnyVaule entity;
		};
		typedef std::vector<EntityPair> EntityContainer;
		typedef EntityContainer::iterator Iterator;
		typedef EntityContainer::const_iterator ConstIterator;

		void Clear();
	private:


		EntityContainer entities_;
	};

}