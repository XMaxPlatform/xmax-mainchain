/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <memory>
namespace unitedb
{
	class IGenericUndo
	{
	public:
		virtual ~IGenericUndo() {}
		virtual void Undo() = 0;
		virtual void Cancel() = 0;
		virtual void Combine() = 0;
		virtual bool Valid() const = 0;
		virtual DBRevision GetRevision() const = 0;
	};

	class DBPatch
	{
	public:

		void Undo();
		void Push();
		void Combine();
		bool Valid() const;
		DBRevision GetRevision() const;
		~DBPatch();
		
		DBPatch(DBPatch&& s);

		friend class FDatabase;
	protected:

		DBPatch(IGenericUndo* ptr);

		std::unique_ptr<IGenericUndo> undo_;
	};

	inline bool DBPatch::Valid() const
	{
		return bool(undo_) && undo_->Valid();
	}

}