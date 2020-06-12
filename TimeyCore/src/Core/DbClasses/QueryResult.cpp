#include"timey_pch.h"
#include "DbClasses/QueryResult.h"


namespace Timey {


	QueryResult::QueryResult(sqlite3_stmt* stmt)
		:_stmt(stmt)
	{
		int ok = sqlite3_step(_stmt);

		while (ok == SQLITE_ROW) {
			_row_count++;
			ok = sqlite3_step(_stmt);
		};
		
		if (ok != SQLITE_DONE) {
			TIMEY_CORE_ERROR("[Sqlite3] Excacuting Failure. {0}", ok);
		};
	};

	

	SQLiteType QueryResult::getColumnType(unsigned int column)
	{
		//A Hack, there is a better way to do this.

		sqlite3_step(_stmt);
		int type = sqlite3_column_type(_stmt, column);
		sqlite3_reset(_stmt);
		return static_cast<SQLiteType>(type);
	}

}