#include "timey_pch.h"
#include "sqlite3.h"
#include <filesystem>

#include"DataBase.h"



namespace Timey {


	void SqliteDb::initDb(const std::string& filePath)
	{	
		namespace fs = std::filesystem;
		if (!fs::is_regular_file(filePath))
		{
			// Database not found, create a new one. 

			int ok = sqlite3_open_v2(filePath.c_str(), &sqliteHandle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
			if (ok != SQLITE_OK) {
				TIMEY_ERROR("[Sqlite3] Failed to open file {0}", filePath.c_str());
				const char* msg = sqlite3_errmsg(sqliteHandle);
				TIMEY_CORE_ERROR("[Sqlite3] {0}", msg);
			}
		};
	

		int ok = sqlite3_open_v2(filePath.c_str(), &sqliteHandle, SQLITE_OPEN_READWRITE, nullptr);
		if (ok != SQLITE_OK) {
			TIMEY_ERROR("[Sqlite3] Failed to open file {0}", filePath.c_str());
			const char* msg = sqlite3_errmsg(sqliteHandle);
			TIMEY_CORE_ERROR("[Sqlite3] {0}", msg);
		}
	}

	void SqliteDb::closeDb()
	{
		sqlite3_close(sqliteHandle);
	}

	int SqliteQuery::bindDb(SqliteDb& db)
	{
		int ok = sqlite3_prepare_v2(db.getRawDbHanle(), query.c_str(), -1, &stmt, nullptr);
		if (ok != SQLITE_OK) {
			TIMEY_CORE_ERROR("[Sqlite3] stmt compile faliure {0}", ok);
			const char* msg = sqlite3_errmsg(db.getRawDbHanle());
			TIMEY_CORE_ASSERT(false, msg);
		};

		prepared = true;
		bindedDb = &db;

		return ok;
	}

	void SqliteQuery::unbindDb()
	{
		sqlite3_finalize(stmt);
		stmt = nullptr;
		bindedDb = nullptr;

		prepared = false;

	}

	int SqliteQuery::bindColumnInteger(uint32_t idx, int value)
	{
		int ok = sqlite3_bind_int(stmt, idx, value);
		bdErrorMsg(ok);

		return ok;
	}

	int SqliteQuery::bindColumnReal(uint32_t idx, double value)
	{
		int ok = sqlite3_bind_double(stmt, idx, value);
		bdErrorMsg(ok);

		return ok;
	}

	int SqliteQuery::bindColumnText(uint32_t idx, const char* value)
	{
		int ok = sqlite3_bind_text(stmt, idx, value, -1, nullptr);
		bdErrorMsg(ok);

		return ok;
	}

	int SqliteQuery::bindColumnNull(uint32_t idx)
	{
		int ok = sqlite3_bind_null(stmt, idx);
		bdErrorMsg(ok);

		return ok;
	}

	int SqliteQuery::bindColumnBlob(uint32_t idx, const void* data, uint32_t size)
	{
		int ok = sqlite3_bind_blob(stmt, idx, data, size, nullptr);
		bdErrorMsg(ok);

		return ok;
	}

	void SqliteQuery::bdErrorMsg(int errcode)
	{
		if ((errcode != SQLITE_OK)) {
			const char* msg = sqlite3_errmsg(bindedDb->getRawDbHanle());
			TIMEY_CORE_ERROR("[Sqlite3] Binding Failure.\n");
			TIMEY_CORE_ASSERT(false, msg);
		};
	}



}