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
				if (!mapped_file::grow(filepath.generic_string().c_str(), mapped_file_size - mapped_file_size))
					BOOST_THROW_EXCEPTION(std::runtime_error("could not grow mapped file to requested size."));
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

		FDatabase(const fs::path& dir, uint64_t managed_file_size)
		{
			init(dir, managed_file_size);
		}

		FDatabase(const fs::path& dir, uint64_t managed_file_size, InitFlag flag)
		{
			if (flag & Discard)
			{
				auto file = dbFilePath(dir);
				if (fs::exists(file))
				{
					fs::remove(file);
				}
			}

			init(dir, managed_file_size);
		}
		~FDatabase()
		{
			Flush();
		}

		virtual void Flush() override
		{
			if (db_file_)
			{
				db_file_->flush();
			}
		}
		virtual mapped_file::segment_manager* GetSegmentManager() const override
		{
			return db_file_->get_segment_manager();
		}

		virtual mapped_file* GetMappdFile() const override
		{
			return db_file_.get();
		}

		virtual UndoSession StartUndo()
		{
			return UndoSession(UndoMgr->StartUndo());
		}

		virtual void PushUndo(const UndoOpArg& arg) override
		{
			UndoMgr->PushUndo(arg);
		}

		virtual void PopUndo() override
		{
			UndoMgr->PopUndo();
		}

		virtual void EnableUndo(bool set) override
		{
			for (auto it : tables_)
			{
				it->SetUndo(set);
			}
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

			tables_.push_back(table);
			tablemap_[code].reset(table);
		}
	private:

		void init(const fs::path& dir, uint64_t managed_file_size)
		{
			if (!fs::exists(dir))
			{
				fs::create_directories(dir);
			}

			db_path_ = dir;
			db_file_path_ = dbFilePath(db_path_);

			db_file_.reset(_OpenMappedFile(db_file_path_, managed_file_size));


			UndoMgr = std::make_unique<UndoManager>(this, GetSegmentManager());
		}

		std::unique_ptr<mapped_file> db_file_;
		fs::path	db_path_;
		fs::path	db_file_path_;

		std::vector< ITable* > tables_;
		std::vector< std::unique_ptr<ITable> > tablemap_;

		std::unique_ptr<UndoManager> UndoMgr;
	};


	Database* Database::InitDB(const fs::path& dir, uint64_t managed_file_size)
	{
		return new FDatabase(dir, managed_file_size);
	}
	Database* Database::InitDB(const fs::path& dir, uint64_t managed_file_size, InitFlag flag)
	{
		return new FDatabase(dir, managed_file_size, flag);
	}

	void Database::DestroyDB(Database* db)
	{
		delete db;
	}

}