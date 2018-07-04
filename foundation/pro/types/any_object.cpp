/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/types/any_object.hpp>

namespace pro
{
	const AnyValue AnyObject::EmptyValue;

	AnyObject::AnyObject()
		: entities_(std::make_shared<EntityContainer>())
	{

	}
	AnyObject::AnyObject(const AnyObject& anys)
		: AnyObject()
	{
		assign(anys);
	}
	AnyObject::AnyObject(AnyObject&& anys)
		: AnyObject()
	{
		assign(std::forward<AnyObject>(anys));
	}

	AnyObject::~AnyObject()
	{

	}

	AnyObject& AnyObject::operator =(const AnyObject& anys)
	{
		assign(anys);
		return *this;
	}
	AnyObject& AnyObject::operator =(AnyObject&& anys)
	{
		assign(std::forward<AnyObject>(anys));
		return *this;
	}

	void AnyObject::assign(const AnyObject& anys)
	{
		*entities_ = *anys.entities_;
	}
	void AnyObject::assign(AnyObject&& anys)
	{
		entities_ = anys.entities_;
		anys.entities_.reset();
	}

	void AnyObject::Clear()
	{
		entities_->clear();
	}

	bool AnyObject::KeyValid(const EntityKey& key) const
	{
		auto it = find(key);
		if (it == end())
		{
			return false;
		}
		return it->value.IsValid();
	}

	size_t AnyObject::Count() const
	{
		return entities_->size();
	}

	const AnyValue& AnyObject::At(const EntityKey& key) const
	{
		auto it = find(key);
		if (it != end())
		{
			return it->value;
		}
		return EmptyValue;
	}

	AnyValue& AnyObject::At(const EntityKey& key)
	{
		auto it = find(key);
		if (it != end())
		{
			return it->value;
		}
		return pushback(key);
	}


	const AnyObject::Entity& AnyObject::GetEntity(size_t index) const
	{
		return (*entities_)[index];	
	}


	AnyObject& AnyObject::Set(const EntityKey& key, const AnyValue& val)
	{
		auto it = find(key);
		if (it != end())
		{
			it->value = val;
		}
		else
		{
			pushback(key) = val;
		}
		return *this;
	}

	AnyObject& AnyObject::Set(EntityKey&& key, AnyValue&& val)
	{
		auto it = find(key);
		if (it != end())
		{
			it->value = std::forward<AnyValue>(val);
		}
		else
		{
			pushback(std::forward<EntityKey>(key)) = std::forward<AnyValue>(val);
		}
		return *this;
	}

	void AnyObject::Emplace(EntityKey&& key, AnyValue&& val)
	{
		auto it = find(key);
		if (it != end())
		{
			it->value = val;
		}
		else
		{
			emplaceback(std::forward<EntityKey>(key)) = std::forward<AnyValue>(val);
		}
		
	}
	AnyObject::Iterator AnyObject::Erase(ConstIterator it)
	{
		return entities_->erase(it);
	}

	bool AnyObject::Remove(const EntityKey& key)
	{
		auto it = find(key);
		if (it != end())
		{
			Erase(it);
			return true;
		}
		return false;
	}

	const AnyValue& AnyObject::operator [] (const EntityKey& key) const
	{
		return At(key);
	}
	AnyValue& AnyObject::operator [] (const EntityKey& key)
	{
		return At(key);
	}
	const AnyValue& AnyObject::operator [] (size_t index) const
	{
		return (*entities_)[index].value;
	}
	AnyValue& AnyObject::operator [] (size_t index)
	{
		return (*entities_)[index].value;
	}

	AnyObject::ConstIterator AnyObject::Begin() const
	{
		return begin();
	}
	AnyObject::Iterator AnyObject::Begin()
	{
		return begin();
	}
	AnyObject::ConstIterator AnyObject::End() const
	{
		return end();
	}
	AnyObject::Iterator AnyObject::End()
	{
		return end();
	}

	AnyObject::ConstIterator AnyObject::begin() const
	{
		return entities_->begin();
	}
	AnyObject::Iterator AnyObject::begin()
	{
		return entities_->begin();
	}
	AnyObject::ConstIterator AnyObject::end() const
	{
		return entities_->end();
	}
	AnyObject::Iterator AnyObject::end()
	{
		return entities_->end();
	}

	AnyObject::Iterator AnyObject::find(const EntityKey& key)
	{
		for (auto it = entities_->begin(); it != entities_->end(); ++it)
		{
			if (it->key == key)
			{
				return it;
			}
		}
		return end();
	}
	AnyObject::ConstIterator AnyObject::find(const EntityKey& key) const
	{
		for (auto it = entities_->begin(); it != entities_->end(); ++it)
		{
			if (it->key == key)
			{
				return it;
			}
		}
		return end();
	}

	AnyValue& AnyObject::emplaceback(EntityKey&& key)
	{
		return entities_->emplace_back(Entity(std::forward<EntityKey>(key))).value;
	}
	AnyValue& AnyObject::pushback(const EntityKey& key)
	{
		return entities_->emplace_back(Entity(key)).value;
	}

	AnyValue& AnyObject::pushback(EntityKey&& key)
	{
		return emplaceback(std::forward<EntityKey>(key));
	}

}