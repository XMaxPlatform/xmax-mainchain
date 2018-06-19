/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/build.hpp>

#include <memory>
#include <vector>
#include <pro/types/generictypes.hpp>

namespace pro
{

	class AnyEntity
	{
	public:
		enum TypeCode
		{
			UI64,
			I64,
			String,
		};
		AnyEntity() = default;
		AnyEntity(const AnyEntity&) = delete;
		AnyEntity& operator = (const AnyEntity &) = delete;

		union Entity
		{

		};
	};

	class AnyValue
	{
	public:
		AnyValue() = default;
		AnyValue(const AnyValue&) = delete;
		AnyValue& operator = (const AnyValue &) = delete;
		~AnyValue();
		typedef string EntityKey;
		struct EntityPair
		{
			EntityKey key;
			AnyEntity entity;
		};
		typedef std::vector<EntityPair> EntityContainer;
		typedef EntityContainer::iterator Iterator;
		typedef EntityContainer::const_iterator ConstIterator;

		void Clear();
	private:


		EntityContainer entities_;
	};

}