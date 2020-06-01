#include"timey_pch.h"
#include "DbClasses/QueryResult.h"


namespace Timey {


	QueryResult::QueryResult(sqlite3_stmt* stmt)
		:_stmt(stmt)
	{
		while (sqlite3_step(_stmt) != SQLITE_DONE) {
			_row_count++;
		}
	}

	SQLiteType QueryResult::getColumnType(unsigned int column)
	{
		return static_cast<SQLiteType>(sqlite3_column_type(_stmt, column));
	}

}