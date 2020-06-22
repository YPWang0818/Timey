#pragma once
#include "timey_pch.h"
#include "sqlite3.h"


namespace Timey {


	enum class SQLiteType {
		integer = SQLITE_INTEGER,
		real = SQLITE_FLOAT, // 64-bit.
		text = SQLITE_TEXT,
		blob = SQLITE_BLOB,
		null = SQLITE_NULL

	};

	inline std::ostream& operator<<(std::ostream& os, const SQLiteType& type) {
		switch (type) {
		case SQLiteType::integer: return (os << "INTEGER");
		case SQLiteType::real: return (os << "REAL");
		case SQLiteType::blob: return (os << "BLOB");
		case SQLiteType::null: return (os << "NULL");
		case SQLiteType::text: return (os << "TEXT");
		};
	};

	class QueryResult {

	public:
		QueryResult(sqlite3_stmt* stmt);
		SQLiteType getColumnType(unsigned int column);

		inline int getColumnCount() const { return sqlite3_column_count(_stmt);  };
		inline int getRowCount() const { return _row_count;  };
		inline bool isEmpty() { return !(_row_count); };

		void DumpAllTable();

		template<class T>
		std::vector<T> getColumn(unsigned int  column) {
		
			TIMEY_CORE_ASSERT(false, "Unsupported column type.");
			return nullptr;
		};

		// template specialization

		template<>
		std::vector<int> getColumn<int>(unsigned int column) {
			if (getColumnType(column) != SQLiteType::integer) {
				TIMEY_CORE_WARN("Inorrect column type. Implcit conversion preformed.");
				TIMEY_CORE_WARN("Convert from {0} to {1}", getColumnType(column), SQLiteType::integer);
			}

			if (this->isEmpty()) {
				TIMEY_CORE_WARN("Empty Query");
				return std::vector<int>();
			}

			std::vector<int> data;
			data.reserve(_row_count);

			for(int i = 1; i <= _row_count; i++){
				sqlite3_step(_stmt);
				data.push_back(sqlite3_column_int(_stmt, column));
			}

			sqlite3_reset(_stmt);

			return data;
		};

		template<>
		std::vector<double> getColumn<double>(unsigned int column) {
			if (getColumnType(column) != SQLiteType::real) {
				TIMEY_CORE_WARN("Inorrect column type. Implcit conversion preformed.");
				TIMEY_CORE_WARN("Convert from {0} to {1}", getColumnType(column), SQLiteType::real);
			}

			if (this->isEmpty()) {
				TIMEY_CORE_WARN("Empty Query");
				return std::vector<double>();
			}

			std::vector<double> data;
			data.reserve(_row_count);

			for (int i = 1; i <= _row_count; i++){
				sqlite3_step(_stmt);
				data.push_back(sqlite3_column_double(_stmt, column));
			}

			sqlite3_reset(_stmt);
			return data ;
		};

		template<>
		std::vector<std::string> getColumn<std::string>(unsigned int column) {
			if (getColumnType(column) != SQLiteType::text) {
				TIMEY_CORE_WARN("Inorrect column type. Implcit conversion preformed.");
				TIMEY_CORE_WARN("Convert from {0} to {1}", getColumnType(column), SQLiteType::text);
			}

			if (this->isEmpty()) {
				TIMEY_CORE_WARN("Empty Query");
				return std::vector<std::string>();
			}

			std::vector<std::string> data;
			data.reserve(_row_count);

			for (int i = 1; i <= _row_count; i++){

				sqlite3_step(_stmt);
				const char* result = reinterpret_cast<const char*>(sqlite3_column_text(_stmt, column));

				size_t count = sqlite3_column_bytes(_stmt, column) / sizeof(char);

				if (!result){
					TIMEY_WARN("Null result found");
					result = "NULL";
					count = 4;
				}

				data.emplace_back(std::string(result, count));
			};

			sqlite3_reset(_stmt);
			return data;
		};

	private:

		int _row_count = 0;
		sqlite3_stmt* _stmt;


	private:

		template<class T>
		void static _print_column(const std::vector<T>& data)
		{
			for (auto it = data.begin(); it != data.end(); ++it) {
				TIMEY_TRACE("{0}", *it);
			}
		}
		
	};



}