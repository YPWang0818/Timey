#include "timey_pch.h"
#include "Query.h"
#include "CoreDataBase.h"

namespace Timey {



	Query::Query(const std::string& query, CoreDataBase* db)
		:_database(db)
	{
		int ok = sqlite3_prepare_v2(_database->getSqliteDb(), query.c_str(), -1, &_stmt, nullptr);
		if (ok != SQLITE_OK) {
			TIMEY_CORE_ERROR("[Sqlite3] Preparing Failure {0}", ok);
			TIMEY_CORE_ASSERT(false, "");
		};
		
	}

	Query::~Query()
	{
		sqlite3_finalize(_stmt);
	}

	void Query::Unbind()
	{
		sqlite3_clear_bindings(_stmt);
	}

}