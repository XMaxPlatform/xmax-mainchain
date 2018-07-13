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

	template<typename _multi_index>
	class TMappedIndex
	{
	public:
		typedef _multi_index MultiIndexType;
		typedef typename MultiIndexType::value_type ObjectType;
		typedef DBAlloc<TMappedIndex> AllocType;

		TMappedIndex(AllocType alloc)
			: indices_(alloc)
		{

		}

		ObjectIDCode GenerateID()
		{
			++counter_;
			return counter_;
		}

		ObjectIDCode counter_ = 0;
		MultiIndexType indices_;
	};


	class ITable
	{
	public:
		virtual ~ITable() {}
	};

	template<typename _multi_index>
	class DBTable : public ITable
	{
	public:
		typedef TMappedIndex<_multi_index> MappedIndex;
		typedef MappedIndex* MappedPtr;
		typedef typename MappedIndex::MultiIndexType MultiIndexType;
		typedef typename MappedIndex::ObjectType ObjectType;
		typedef typename MappedIndex::AllocType AllocType;

		static std::string TableName()
		{
			static std::string type_name = boost::core::demangle(typeid(typename ObjectType).name()) + "Table";
			return type_name;
		}

		template<typename Constructor>
		ObjPtr<ObjectType> NewObject(Constructor&& c)
		{
			ObjectIDCode id = ptr_->GenerateID();
			auto constructor = [&](ObjectType& v) {
				c(v);
				v.id_ = id;
			};

			auto result = GetMapped().emplace(constructor, GetMapped().get_allocator());

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
			auto result = GetMapped().modify(GetMapped().iterator_to(obj.Get()), update);
			if (!result)
				BOOST_THROW_EXCEPTION(std::logic_error("Could not Update object, most likely a uniqueness constraint was violated."));
		}

		void DeleteObject(const ObjPtr<ObjectType>& obj)
		{
			if (obj)
			{
				GetMapped().erase(GetMapped().iterator_to(obj.Get()));
			}
		}

		//template<typename OrderedTag>
		//const typename MultiIndexType::index<OrderedTag>::type& GetOrderIndex() const
		//{
		//	return indices_.get<OrderedTag>();
		//}
		template<typename OrderedTag>
		auto GetOrderIndex() const -> decltype( ((const MultiIndexType*)(nullptr))->template get<OrderedTag>() )
		{
			return GetMapped().template get<OrderedTag>();
		}

	protected:
		DBTable(MappedPtr ptr)
			: ptr_(ptr)
		{

		}

		inline MultiIndexType & GetMapped()
		{
			return ptr_->indices_;
		}
		inline const MultiIndexType & GetMapped() const
		{
			return ptr_->indices_;
		}
		MappedPtr ptr_ = nullptr;
		//template<typename Get>
	};

	struct ByObjectID;

	#define INDEXED_BY_OBJECT_ID boost::multi_index::ordered_unique<boost::multi_index::tag<ByObjectID>, boost::multi_index::member<DBObjectBase, ObjectIDCode, &DBObjectBase::id_>>

	template<typename _Object, typename... _Args>
	using DBTableDeclaration = boost::multi_index_container<_Object, _Args..., DBAlloc<_Object> >;




}