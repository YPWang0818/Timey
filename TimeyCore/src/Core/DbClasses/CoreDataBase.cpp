#include "timey_pch.h"
#include"CoreDataBase.h"
#include"Query.h"

namespace Timey {


	CoreDataBase::CoreDataBase(const std::string& file_name)
		:_file_name(file_name)
	{
		// Needs to find if the database exists, if not, create one with the correct scheme.

		sqlite3_open(_file_name.c_str(), &_db);

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

	void CoreDataBase::CreateQuery(const std::string& name, const std::string& stmt)
	{
		

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