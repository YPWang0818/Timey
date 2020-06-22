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

	void QueryResult::DumpAllTable() {
		

		int col_num = this->getColumnCount();

		if (this->isEmpty()) {
			TIMEY_INFO("Empty Query");
			return;
		}

		for (int i = 0; i < col_num; i++) {

			TIMEY_INFO("---- column {0} -----", i);

			switch (this->getColumnType(i))
			{
			case Timey::SQLiteType::integer:   _print_column<int>(this->getColumn<int>(i)); break;
			case Timey::SQLiteType::real:	   _print_column<double>(this->getColumn<double>(i)); break;
			case Timey::SQLiteType::text:     _print_column<std::string>(this->getColumn<std::string>(i)); break;
			case Timey::SQLiteType::blob:	  TIMEY_INFO("blob unsupported."); break;
			case Timey::SQLiteType::null:	  TIMEY_INFO("NULL column"); break;

			}

		}

	};
}