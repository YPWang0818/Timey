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

	int SqliteQuery::compile(SqliteDb& db)
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

	void SqliteQuery::detach()
	{
		sqlite3_finalize(stmt);
		stmt = nullptr;
		bindedDb = nullptr;
		prepared = false;

	}

	int SqliteQuery::bindColumnInteger(uint32_t idx, int value)
	{
		if (!prepared) { TIMEY_ASSERT(false, "The statement is not prepared."); };
		int ok = sqlite3_bind_int(stmt, idx, value);
		bdErrorMsg(ok);

		return ok;
	}

	int SqliteQuery::bindColumnReal(uint32_t idx, double value)
	{
		if (!prepared) { TIMEY_ASSERT(false, "The statement is not prepared."); };

		int ok = sqlite3_bind_double(stmt, idx, value);
		bdErrorMsg(ok);

		return ok;
	}

	int SqliteQuery::bindColumnText(uint32_t idx, const char* value)
	{
		if (!prepared) { TIMEY_ASSERT(false, "The statement is not prepared."); };

		int ok = sqlite3_bind_text(stmt, idx, value, -1, nullptr);
		bdErrorMsg(ok);

		return ok;
	}

	int SqliteQuery::bindColumnNull(uint32_t idx)
	{
		if (!prepared) { TIMEY_ASSERT(false, "The statement is not prepared."); };
		int ok = sqlite3_bind_null(stmt, idx);
		bdErrorMsg(ok);

		return ok;
	}

	int SqliteQuery::bindColumnBlob(uint32_t idx, const void* data, uint32_t size)
	{
		if (!prepared) { TIMEY_ASSERT(false, "The statement is not prepared."); };
		int ok = sqlite3_bind_blob(stmt, idx, data, size, nullptr);
		bdErrorMsg(ok);

		return ok;
	}

	int SqliteQuery::unbindAllColumns()
	{
		return sqlite3_clear_bindings(stmt);
	
	}

	Ref<SqliteTable> SqliteQuery::exec()
	{
		TIMEY_CORE_ASSERT(( bindedDb != nullptr), "No database binded.");
		
		int ok = sqlite3_step(stmt);
		if ((ok != SQLITE_ROW) && (ok != SQLITE_DONE)) {
			TIMEY_CORE_ERROR("Execute query failure. ({0})", ok);
			return nullptr;
		};


		sqlite3_reset(stmt);
		if (!sqlite3_column_count(stmt)) return nullptr;

		Ref<SqliteTable> tb{ new SqliteTable(stmt) };

		return tb;
	}

	Ref<SqliteRow> SqliteTable::getCurrentRow()
	{

		if (!currentRow) return nullptr;		
		if (cache.find(currentRow) != cache.end()) { return cache[currentRow]; };

		Ref<SqliteRow> row = CreateRef<SqliteRow>();
		row->reserve(colCount);

		for (int col = 0; col < colCount; col++) {

			//This switch statment should have explicit control over the type of SqliteEntry it creates.

			switch (SqliteTable::getTypeFromNativeType(sqlite3_column_type(stmt, col))) {
			case SQLType::integer: 
				row->emplace_back(getEntryInt(col)); 
				break;

			case SQLType::blob: 
				const void* data; std::size_t sz; 
				data = getEntryBlob(col, sz);
				row->emplace_back(data, sz);
				break;

			case SQLType::null:;
				row->emplace_back();
				break;

			case SQLType::real:
				row->emplace_back(getEntryReal(col));
				break;

			case SQLType::text:
				const char* cdata; std::size_t csz;
				cdata = getEntryText(col, csz);
				row->emplace_back(cdata, csz);
				break;

			default:
				TIMEY_CORE_ERROR("Undefined SQLType.");
				break;
			};
		}

		cache[currentRow] = row;

		return row;
	}

	int SqliteTable::nextRow()
	{
		int ok = sqlite3_step(stmt);
		currentRow++;

		if ((ok != SQLITE_ROW) && (ok != SQLITE_DONE))
		{
			TIMEY_CORE_ERROR("Execute query failure.({0})", ok);
		};

		return ok;
	};

	void SqliteTable::resetRow()
	{
		currentRow = 0;
		sqlite3_reset(stmt);

	};

	Ref<SqliteColumn> SqliteTable::getColumn(std::size_t col)
	{
		uint32_t currentRowSaved = currentRow;

		resetRow(); // reset to intitial row status. 

		Ref<SqliteColumn> column = CreateRef<SqliteColumn>();
		TIMEY_CORE_ASSERT(col < colCount, "Column out of bound.");

		while (nextRow() == SQLITE_ROW) {
			column->push_back((* (getCurrentRow()) )[col]);
		}

		for (int col = 0; col < currentRowSaved; col++)
		{
			sqlite3_step(stmt); // restore the row status. 		
		};

		currentRow = currentRowSaved;
		return column;

	}

	int64_t SqliteTable::getEntryInt(uint32_t col)
	{
		return sqlite3_column_int64(stmt, col);
	}

	double SqliteTable::getEntryReal(uint32_t col)
	{
		 return sqlite3_column_double(stmt, col);
	}

	const char* SqliteTable::getEntryText(uint32_t col, std::size_t& sz)
	{
		sz = sqlite3_column_bytes(stmt, col) + 1;
		return (const char*)(sqlite3_column_text(stmt, col));
	};

	const void* SqliteTable::getEntryBlob(uint32_t col, std::size_t& sz)
	{
		sz = sqlite3_column_bytes(stmt, col);
		return sqlite3_column_blob(stmt, col);
	}

	SQLType SqliteTable::getTypeFromNativeType(int type)
	{
		switch (type) {

		case SQLITE_INTEGER: return SQLType::integer;
		case SQLITE_FLOAT: return SQLType::real;
		case SQLITE_TEXT: return SQLType::text;
		case SQLITE_BLOB: return SQLType::blob;
		case SQLITE_NULL: return SQLType::null;
		default: return SQLType::undef;

		};
	}

	std::string SqliteTable::toString()
	{
		std::stringstream ss;

		while (nextRow() == SQLITE_ROW) {

			Ref<SqliteRow> row = getCurrentRow();
			for (auto& e : *row) {
				switch (e.type) {
				case SQLType::integer:
					ss << e.data.i << " | ";
					break;
				case SQLType::real:
					ss << e.data.d << " | ";
					break;
				case SQLType::null:
					ss << "NULL" << " | ";
					break;
				case SQLType::text:
					ss << e.data.c << " | ";
					break;
				case SQLType::blob:
					ss << "(Blob item)" << " | ";
					break;
				default:
					break;
				}
			};

			ss << " \n ";

		}

		resetRow();

		return ss.str();
	};



	void SqliteQuery::bdErrorMsg(int errcode)
	{
		if ((errcode != SQLITE_OK)) {
			const char* msg = sqlite3_errmsg(bindedDb->getRawDbHanle());
			TIMEY_CORE_ERROR("[Sqlite3] Binding Failure.\n");
			TIMEY_CORE_ASSERT(false, msg);
		};
	}



}