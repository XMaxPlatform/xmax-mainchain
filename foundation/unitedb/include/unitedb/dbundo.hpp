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

		virtual UndoRevision GetRevision() const = 0;
	};

	class UndoSession
	{
	public:

		void Undo();
		void Cancel();
		UndoRevision GetRevision() const;
		~UndoSession();
		

		UndoSession(UndoSession&& s);

		friend class FDatabase;
	protected:

		UndoSession(IGenericUndo* ptr);

		std::unique_ptr<IGenericUndo> undo_;
	};

}