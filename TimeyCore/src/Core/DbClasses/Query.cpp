#include "timey_pch.h"
#include "Query.h"
#include "CoreDataBase.h"

namespace Timey {



	Query::Query(const std::string& query, CoreDataBase* db)
		:_database(db)
	{
		int ok = sqlite3_prepare_v2(_database->getSqliteDb(), query.c_str(), -1, &_stmt, nullptr);
		if (ok != SQLITE_OK) {
			TIMEY_CORE_ERROR("[Sqlite3] stmt compile faliure {0}", ok);
			const char* msg = sqlite3_errmsg(_database->getSqliteDb());
			TIMEY_CORE_ASSERT(false, msg);
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

	QueryResult* Query::Exec()
	{
		return new QueryResult(_stmt);
	}

	void Query::ExecOnceNoRes() {
		
		int ok = sqlite3_step(_stmt);

		if (ok != SQLITE_DONE) {
			if (ok == SQLITE_ROW) { TIMEY_CORE_WARN("Some query result is found."); }
			else { 
				const char* msg = sqlite3_errmsg(_database->getSqliteDb());
				TIMEY_CORE_ERROR("[Sqlite3] Excute failure. : {0}", msg);
			};
		};

		sqlite3_reset(_stmt);
	}

	void Query::_binding_errmsg(int ok)
	{
		if ((ok != SQLITE_OK)) {
			const char* msg = sqlite3_errmsg(_database->getSqliteDb());
			TIMEY_CORE_ERROR("[Sqlite3] Binding Failure.");
			TIMEY_CORE_ASSERT(false, msg)
		};
		
	}
	
}