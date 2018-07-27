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
	/**
	*  @brief An dictionary store any_value.
	*
	*  An dictionary store any value, any value has unique name, we can search by name.
	*/
	class AnyObject
	{
	public:
		/**
		* Constructs with null.
		*/
		AnyObject();
		/**
		* Constructs with another object.
		* @param[in] anys another object.
		*/
		AnyObject(const AnyObject& anys);
		/**
		* Constructs with another right object.
		* @param[in] anys another right object.
		*/
		AnyObject(AnyObject&& anys);
		~AnyObject();

		/**
		* EntityKey type.
		*/
		typedef string EntityKey;

		/**
		* Entity struct.
		*/
		struct Entity
		{
			/**
			* Entity key.
			*/
			EntityKey key;
			/**
			* Object data.
			*/
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
		typedef EntityContainer* EntityContainerPtr;
		typedef std::shared_ptr<EntityContainer> SharedEntitys;
		typedef EntityContainer::iterator Iterator;
		typedef EntityContainer::const_iterator ConstIterator;
		
		/**
		*  get value by key, if not found, return empty value..
		* @return the value.
		*/
		const AnyValue& At(const EntityKey& key) const;

		/**
		* get value by key, if not found, create new value named key.
		* @return the value.
		*/
		AnyValue& At(const EntityKey& key);

		/**
		* set a named value.
		* @param[in] key the name of value.
		* @param[in] val the value.
		* @return self.
		*/
		AnyObject& Set(const EntityKey& key, const AnyValue& val);
		/**
		* set a named value.
		* @param[in] key the name of value.
		* @param[in] val the value.
		* @return self.
		*/
		AnyObject& Set(EntityKey&& key, AnyValue&& val);

		/**
		* emplace a named value.
		* @param[in] key the name of value.
		* @param[in] val the value.
		*/
		void Emplace(EntityKey&& key, AnyValue&& val);

		/**
		* Erase a value in where.
		* @param[in] wh where to erase.
		*/
		Iterator Erase(ConstIterator wh);

		/**
		* Remove a value by key.
		* @param[in] key the name of value.
		* @return if remove return true, other way return false.
		*/
		bool Remove(const EntityKey& key);

		/**
		* Remove all values.
		*/
		void Clear();

		/**
		* check if a key valid.
		* if key found and the value of key is valid, return true, otherwise return false.
		* @param[in] key the name of value.
		* @return if key valid return true, otherwise return false.
		*/
		bool KeyValid(const EntityKey& key) const;

		/**
		* the some as fuction At()
		*/
		const AnyValue& operator [] (const EntityKey& key) const;
		/**
		* the some as fuction At()
		*/
		AnyValue& operator [] (const EntityKey& key);

		/**
		* get a value by index.
		* @return the value.
		*/
		const AnyValue& operator [] (size_t index) const;
		/**
		* get a value by index.
		* @return the value.
		*/
		AnyValue& operator [] (size_t index);

		/**
		* operator = AnyObject&&, and return self.
		* @param[in] anys AnyObject.
		* @return self.
		*/
		AnyObject& operator =(const AnyObject& anys);
		/**
		* operator = AnyObject&&, and return self.
		* @param[in] anys right AnyObject.
		* @return self.
		*/
		AnyObject& operator =(AnyObject&& anys);

		/**
		* get the const iteractor of begin.
		* @return const iteractor.
		*/
		ConstIterator Begin() const;
		/**
		* get the const iteractor of end.
		* @return const iteractor.
		*/
		ConstIterator End() const;

		/**
		* get Entity by index. if not found, throw
		* @return const Entity&.
		*/
		const Entity& GetEntity(size_t index) const;
		/**
		* get Entity count
		* @return const Entity&.
		*/
		size_t Count() const;
	public: // for std.
		ConstIterator begin() const;

		ConstIterator end() const;

	protected:

		void assign(const AnyObject& anys);
		void assign(AnyObject&& anys);

		Iterator Begin();	
		Iterator End();
		Iterator begin();
		Iterator end();
		Iterator find(const EntityKey& key);
		ConstIterator find(const EntityKey& key) const;

		AnyValue& emplaceback(EntityKey&& key);
		AnyValue& pushback(const EntityKey& key);
		AnyValue& pushback(EntityKey&& key);

		SharedEntitys entities_;

		static const AnyValue EmptyValue;
	};

}