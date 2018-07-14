/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <unitedb/typebase.hpp>
#include <unitedb/dbobject.hpp>

namespace unitedb
{
	class Database;




	template<typename _multi_index>
	class TMappedIndex
	{
	public:
		typedef _multi_index ContainerType;
		typedef typename ContainerType::value_type ObjectType;
		typedef DBAlloc<TMappedIndex> AllocType;

		TMappedIndex(AllocType alloc)
			: indices_(alloc)
			, this_size(sizeof(*this))
			, idxs_size(sizeof(typename ContainerType::node_type))
		{

		}

		void Check()
		{
			if (sizeof(*this) != this_size || sizeof(typename ContainerType::node_type) != idxs_size)
			{
				BOOST_THROW_EXCEPTION(std::runtime_error("content of memory does not match data expected by executable"));
			}
		}

		inline ObjectIDCode GenerateID()
		{
			++id_counter_;
			return id_counter_;
		}

		inline ContainerType& GetIndices()
		{
			return indices_;
		}

		inline const ContainerType& GetIndices() const
		{
			return indices_;
		}

	private:
		ContainerType indices_;
		ObjectIDCode id_counter_ = 0;
		const uint64_t this_size = 0;
		const uint64_t idxs_size = 0;
	};

	template<typename _multi_index>
	class DBTable : public ITable
	{
	public:
		typedef TMappedIndex<_multi_index> MappedIndex;
		typedef MappedIndex* MappedPtr;
		typedef typename MappedIndex::ContainerType MultiIndexType;
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
		DBTable(IDatabase* owner, MappedPtr ptr)
			: owner_(owner)
			, ptr_(ptr)
		{

		}

		inline MultiIndexType& GetMapped()
		{
			return ptr_->GetIndices();
		}
		inline const MultiIndexType& GetMapped() const
		{
			return ptr_->GetIndices();
		}
		MappedPtr ptr_ = nullptr;
		IDatabase* owner_ = nullptr;
		//template<typename Get>
	};

	struct ByObjectID;

	#define INDEXED_BY_OBJECT_ID boost::multi_index::ordered_unique<boost::multi_index::tag<ByObjectID>, boost::multi_index::member<DBObjectBase, ObjectIDCode, &DBObjectBase::id_>>

	template<typename _Object, typename... _Args>
	using DBTableDeclaration = boost::multi_index_container<_Object, _Args..., DBAlloc<_Object> >;




}