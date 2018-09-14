/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <unitedb/undomanager.hpp>
#include <unitedb/database.hpp>
namespace unitedb
{

	static mapped_file* _OpenMappedFile(const fs::path filepath, uint64_t mapped_file_size)
	{
		if (fs::exists(filepath))
		{
			auto file_size = fs::file_size(filepath);
			if (mapped_file_size > file_size)
			{
				if (!mapped_file::grow(filepath.generic_string().c_str(), mapped_file_size - file_size))
					DB_THROW(std::runtime_error("could not grow mapped file to requested size."));
			}

			return new mapped_file(inpr::open_only, filepath.generic_string().c_str());
		}
		else
		{
			return (new mapped_file(inpr::create_only,
				filepath.generic_string().c_str(), mapped_file_size,
#ifdef WIN32
				0, NULL
#else
				0, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
#endif
			));
		}
	}

	static fs::path dbFilePath(const fs::path& work_path)
	{
		return work_path / "unitedb.bin";
	}



	class FDatabase : public Database, public IDatabase
	{
	public:
		FDatabase()
		{

		}
		~FDatabase()
		{
			db_file_.reset();
		}

		void Init(const fs::path& dir, uint64_t managed_file_size)
		{
			initImpl(dir, managed_file_size, InitFlag::NoFlag);
		}

		void Init(const fs::path& dir, uint64_t managed_file_size, InitFlag flag)
		{
			initImpl(dir, managed_file_size, flag);
		}

		// Database API 

		virtual DBPatch StartUndo() override
		{
			return DBPatch(UndoMgr->StartUndo());
		}

		virtual void Flush() override
		{
			if (db_file_)
			{
				db_file_->flush();
			}
		}

		virtual void Close() override
		{
			if (db_file_)
			{
				db_file_.reset();
			}
		}

		virtual bool Commit(DBRevision rev) override
		{
			return UndoMgr->Commit(rev);
		}

		virtual DBRevision GetTopRevision() const override
		{
			return UndoMgr->TopRevision();
		}

		virtual DBRevision GetLastCommit() const override
		{
			return UndoMgr->LastCommit();
		}

		// Database API end.

		virtual mapped_file::segment_manager* GetSegmentManager() const override
		{
			return db_file_->get_segment_manager();
		}

		virtual mapped_file* GetMappdFile() const override
		{
			return db_file_.get();
		}


		virtual void PushUndo(const UndoOpArg& arg) override
		{
			UndoMgr->PushUndo(arg);
		}

		virtual void LastUpdateFailure(ObjIDCode id) override
		{
			UndoMgr->LastUpdateFailure(id);
		}

		virtual void OnStartUndo(DBRevision revision) override
		{
			for (auto it : table_insts_)
			{
				it->StartUndo(revision);
			}
		}

		virtual void OnCombine(DBRevision revision) override
		{
			for (auto it : table_insts_)
			{
				it->Combine(revision);
			}
		}

		virtual void OnCommmit(DBRevision revision) override
		{
			for (auto it : table_insts_)
			{
				it->Commit(revision);
			}
		}

		virtual ITable* GetTable(ObjectTypeCode code) override 
		{
			DB_ASSERT(code < tablemap_.size());
			return tablemap_[code].get();
		}

	protected:
		virtual mapped_file* getMappdFile() const override
		{
			return db_file_.get();
		}

		virtual IDatabase* castDatabase() override
		{
			return this;
		}

		virtual bool tableUsed(ObjectTypeCode code) const override
		{
			return (code < tablemap_.size() && tablemap_[code]);
		}

		virtual ITable* getTableInternal(ObjectTypeCode code) const override
		{
			return tablemap_[code].get();
		}
		virtual void setTableInternal(ObjectTypeCode code, ITable* table) override
		{
			if (tablemap_.size() <= code)
			{
				tablemap_.resize(code + 1);
			}

			table_insts_.push_back(table);
			tablemap_[code].reset(table);
		}

		virtual DBRevision TopRevision() const
		{
			return UndoMgr->TopRevision();
		}

	private:

		void initImpl(const fs::path& dir, uint64_t managed_file_size, InitFlag flag)
		{
			db_path_ = dir;
			db_file_path_ = dbFilePath(db_path_);

			if (!fs::exists(db_path_))
			{
				fs::create_directories(db_path_);
			}

			if (flag & Discard)
			{
				if (fs::exists(db_file_path_))
				{
					fs::remove(db_file_path_);
				}
			}
			db_file_.reset(_OpenMappedFile(db_file_path_, managed_file_size));


			UndoMgr = std::make_unique<UndoManager>(this, GetSegmentManager());

			UndoMgr->Init();
		}


		std::unique_ptr<mapped_file> db_file_;
		fs::path	db_path_;
		fs::path	db_file_path_;

		std::vector< ITable* > table_insts_;
		std::vector< std::unique_ptr<ITable> > tablemap_;

		std::unique_ptr<UndoManager> UndoMgr;
	};


	Database* Database::InitDB(const fs::path& dir, uint64_t managed_file_size)
	{
		FDatabase* db = new FDatabase();
		db->Init(dir, managed_file_size);
		return db;
	}
	Database* Database::InitDB(const fs::path& dir, uint64_t managed_file_size, InitFlag flag)
	{
		FDatabase* db = new FDatabase();
		db->Init(dir, managed_file_size, flag);

		return db;
	}

	void Database::DestroyDB(Database* db)
	{
		delete db;
	}

}