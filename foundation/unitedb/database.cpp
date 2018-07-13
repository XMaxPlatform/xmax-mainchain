/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
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



	Database::Database(const fs::path& dir, uint64_t managed_file_size)
	{
		init(dir, managed_file_size);
	}

	Database::Database(const fs::path& dir, uint64_t managed_file_size, InitFlag flag)
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
	void Database::init(const fs::path& dir, uint64_t managed_file_size)
	{
		if (!fs::exists(dir))
		{
			fs::create_directories(dir);
		}

		db_path_ = dir;
		db_file_path_ = dbFilePath(db_path_);

		db_file_.reset(_OpenMappedFile(db_file_path_, managed_file_size));
	}

	Database::~Database()
	{
		Flush();
	}

	void Database::Flush()
	{
		if (db_file_)
		{
			db_file_->flush();
		}
	}
}