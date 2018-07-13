/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <unitedb/dbobject.hpp>

namespace unitedb
{
	class Database;
	class ITable
	{
	public:
		virtual ~ITable() {}
	};

	class DBTableBase : public ITable
	{

	public:
		ObjectIDCode GenerateID()
		{
			++counter_;
			return counter_;
		}

	private:
		ObjectIDCode counter_ = 0;
	};

	template<typename _multi_index>
	class DBTable : public DBTableBase
	{
	public:
		typedef _multi_index MultiIndexType;
		typedef typename MultiIndexType::value_type ObjectType;
		typedef DBAlloc<DBTable> AllocType;

		static std::string TableName()
		{
			static std::string type_name = boost::core::demangle(typeid(typename ObjectType).name()) + "Table";
			return type_name;
		}

		DBTable(DBAlloc<ObjectType> alloc)
			: indices_(alloc)
		{

		}

		template<typename Constructor>
		ObjPtr<ObjectType> NewObject(Constructor&& c)
		{
			ObjectIDCode id = GenerateID();
			auto constructor = [&](ObjectType& v) {
				c(v);
				v.id_ = id;
			};

			auto result = indices_.emplace(constructor, indices_.get_allocator());

			if (!result.second) {
				BOOST_THROW_EXCEPTION(std::logic_error("Could not insert object, most likely a uniqueness constraint was violated"));
			}
			return ObjPtr<ObjectType>::MakePtr(result.first.operator->());
		}


		template<typename OrderedTag, typename Key>
		ObjPtr<ObjectType> FindObject(const Key& k) const
		{
			auto& idx = GetOrderIndex<OrderedTag>();

			auto it = idx.find(k);

			if (it != idx.end())
			{
				return ObjPtr<ObjectType>::MakePtr(&(*it));
			}

			return ObjPtr<ObjectType>::MakePtr(nullptr);
		}

		template<typename UpdateFunc>
		void UpdateObject(const ObjPtr<ObjectType>& obj, UpdateFunc&& update)
		{
			auto result = indices_.modify(indices_.iterator_to(obj.Get()), update);
			if (!result)
				BOOST_THROW_EXCEPTION(std::logic_error("Could not Update object, most likely a uniqueness constraint was violated."));
		}

		void DeleteObject(const ObjPtr<ObjectType>& obj)
		{
			indices_.erase(indices_.iterator_to(obj.Get()));
		}

		//template<typename OrderedTag>
		//const typename MultiIndexType::index<OrderedTag>::type& GetOrderIndex() const
		//{
		//	return indices_.get<OrderedTag>();
		//}
		template<typename OrderedTag>
		auto GetOrderIndex() const -> decltype( ((const MultiIndexType*)(nullptr))->template get<OrderedTag>() )
		{
			return indices_.template get<OrderedTag>();
		}

		//template<typename Get>


		friend class Database;

	protected:
		MultiIndexType indices_;

	};

	struct ByID;

	#define INDEXED_BY_ID boost::multi_index::ordered_unique<boost::multi_index::tag<ByID>, boost::multi_index::member<DBObjectBase, ObjectIDCode, &DBObjectBase::id_>>

	template<typename _Object, typename... _Args>
	using DBTableDeclaration = boost::multi_index_container<_Object, _Args..., DBAlloc<_Object> >;


}