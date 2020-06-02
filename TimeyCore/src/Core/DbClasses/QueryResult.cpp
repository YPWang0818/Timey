#include"timey_pch.h"
#include "DbClasses/QueryResult.h"


namespace Timey {


	QueryResult::QueryResult(sqlite3_stmt* stmt)
		:_stmt(stmt)
	{
		while (sqlite3_step(_stmt) == SQLITE_ROW) {
			_row_count++;
		}


	}

	SQLiteType QueryResult::getColumnType(unsigned int column)
	{
		//A Hack, there is a better way to do this.

		sqlite3_step(_stmt);
		int type = sqlite3_column_type(_stmt, column);
		sqlite3_reset(_stmt);
		return static_cast<SQLiteType>(type);
	}

}