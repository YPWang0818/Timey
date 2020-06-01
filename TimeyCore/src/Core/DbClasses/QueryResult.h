#pragma once
#include "sqlite3.h"

namespace Timey {

	enum class SQLiteType {
		integer = SQLITE_INTEGER,
		real = SQLITE_FLOAT, // 64-bit.
		text = SQLITE_TEXT,
		blob = SQLITE_BLOB,
		null = SQLITE_NULL

	};

	class QueryResult {

	public:
		QueryResult(sqlite3_stmt* stmt);
		SQLiteType getColumnType(unsigned int column);

		inline int getColumnCount() const {return sqlite3_column_count(_stmt)};
		inline int getRowCount() const { return _row_count;  };
		
		template<class T>
		T* getColumn(unsigned int  column) {
		
			TIMEY_CORE_ASSERT(false, "Unsupported column type.");
	
			return (T*)nullptr;
		};

		// template specialization

		template<>
		int* getColumn<int>(unsigned int column) {


		};
	private:

		int _row_count = 0;
		sqlite3_stmt* _stmt;

	};




}