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

		virtual DBRevision GetRevision() const = 0;
	};

	class UndoPatch
	{
	public:

		void Undo();
		void Cancel();
		DBRevision GetRevision() const;
		~UndoPatch();
		

		UndoPatch(UndoPatch&& s);

		friend class FDatabase;
	protected:

		UndoPatch(IGenericUndo* ptr);

		std::unique_ptr<IGenericUndo> undo_;
	};

}