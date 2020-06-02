#include "timey_pch.h"
#include"CoreDataBase.h"
#include"Query.h"

namespace Timey {


	CoreDataBase::CoreDataBase(const std::string& file_name)
		:_file_name(file_name)
	{
		// Needs to find if the database exists, if not, create one with the correct scheme.
		int ok = sqlite3_open(_file_name.c_str(), &_db);
		if (ok != SQLITE_OK) {
			TIMEY_ERROR("[Sqlite3] Failed to open file {0}", _file_name.c_str());
			const char* msg = sqlite3_errmsg(_db);
			TIMEY_CORE_ERROR("[Sqlite3] {0}", msg);
		}
	}

	CoreDataBase::~CoreDataBase() {

		sqlite3_close(_db);

	}

	void CoreDataBase::AddQuery(const std::string& name, Query& query)
	{
		if (_query_cache.find(name) != _query_cache.end()) {
			TIMEY_CORE_WARN("Overriding existing query. {0}", name.c_str());
		};

		_query_cache[name] = &query ;
	}

	void CoreDataBase::RemoveQuery(const std::string& name)
	{
		if (_query_cache.find(name) == _query_cache.end() ) {

			TIMEY_CORE_WARN("Cannot find the query to be deleted.  {0}", name.c_str());
			return;
		}
		else {
			_query_cache.erase(name);
		}
	}
}