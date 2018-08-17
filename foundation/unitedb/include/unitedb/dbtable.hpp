/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <unitedb/dbobject.hpp>
#include <unitedb/dbundoop.hpp>

namespace unitedb
{
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

		inline ObjIDCode GenerateID()
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
		ObjIDCode id_counter_ = 0;
		const uint64_t this_size = 0;
		const uint64_t idxs_size = 0;
	};

	template<typename _multi_index>
	class DBTable : public IDBTable
	{
	public:
		typedef TMappedIndex<_multi_index> MappedIndex;
		typedef MappedIndex* MappedPtr;
		typedef typename MappedIndex::ContainerType MultiIndexType;
		typedef typename MappedIndex::ObjectType ObjectType;
		typedef typename MappedIndex::AllocType AllocType;
		typedef typename ObjectType::ObjectID ObjectID;

		static std::string TableName()
		{
			static std::string type_name = boost::core::demangle(typeid(typename ObjectType).name()) + "Table";
			return type_name;
		}



		template<typename OrderedTag, typename T2>
		ObjPtr<ObjectType> FindObject(const TObjectID<T2>& k) const
		{
			return FindObject<OrderedTag>(k.GetValue());
		}

		template<typename T2>
		ObjPtr<ObjectType> FindObject(const TObjectID<T2>& k) const
		{
			return FindObjectImpl<ByObjectID>(k.GetValue());
		}

		template<typename OrderedTag, typename Key>
		ObjPtr<ObjectType> FindObject(const Key& k) const
		{
			return FindObjectImpl<OrderedTag>(k);
		}

		template<typename Constructor>
		ObjPtr<ObjectType> NewObject(Constructor&& c)
		{
			ObjIDCode id = ptr_->GenerateID();
			auto constructor = [&](ObjectType& v) {
				c(v);
				v.__objid = id;
			};

			auto result = GetMapped().emplace(constructor, GetMapped().get_allocator());

			if (!result.second) {
				BOOST_THROW_EXCEPTION(std::logic_error("Could not insert object, most likely a uniqueness constraint was violated"));
			}
			const ObjectType* ptr = result.first.operator->();
			PushUndo(UndoOp::Create, ptr);
			return ObjPtr<ObjectType>::MakePtr(ptr);
		}

		template<typename UpdateFunc>
		void UpdateObject(const ObjPtr<ObjectType>& obj, UpdateFunc&& update)
		{
			updateObject(obj.Get(), update);
		}

		void DeleteObject(const ObjPtr<ObjectType>& obj)
		{
			if (obj)
			{
				PushUndo(UndoOp::Delete, obj.Ptr());
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

		ObjectTypeCode GetObjectTypeCode() const
		{
			return ObjectType::TypeCode;
		}

	protected:

		template<typename OrderedTag, typename Key>
		ObjPtr<ObjectType> FindObjectImpl(const Key& k) const
		{
			auto& idx = GetOrderIndex<OrderedTag>();

			auto it = idx.find(k);

			if (it != idx.end())
			{
				return ObjPtr<ObjectType>::MakePtr(&(*it));
			}

			return ObjPtr<ObjectType>::MakePtr(nullptr);
		}

		template<typename OrderedTag>
		ObjPtr<ObjectType> FindObject(ObjIDCode c) const
		{
			return FindObjectImpl<OrderedTag>(c);
		}


		DBTable( MappedPtr ptr)
			: ptr_(ptr)
		{

		}
		template<typename UpdateFunc>
		void updateObject(const ObjectType& obj, UpdateFunc&& update)
		{
			PushUndo(UndoOp::Update, &obj);
			auto result = GetMapped().modify(GetMapped().iterator_to(obj), update);
			if (!result)
			{
				LastUpdateFailure(DBObjBase::__getObjidcode(obj));
				BOOST_THROW_EXCEPTION(std::logic_error("Could not Update object, most likely a uniqueness constraint was violated."));
			}	
		}

		virtual void PushUndo(UndoOp::UndoCode code, const DBObjBase* undo)
		{

		}

		virtual void LastUpdateFailure(ObjIDCode id)
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
	};

	struct ByObjectID;

	#define INDEXED_BY_OBJECT_ID boost::multi_index::ordered_unique<boost::multi_index::tag<ByObjectID>, boost::multi_index::member<DBObjBase, ObjIDCode, &DBObjBase::__objid>>

	template<typename _Object, typename... _Args>
	using DBTableDeclaration = boost::multi_index_container<_Object, _Args..., DBAlloc<_Object> >;




}