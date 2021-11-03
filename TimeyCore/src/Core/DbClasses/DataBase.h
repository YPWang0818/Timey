#pragma once
#include "timey_pch.h"
#include "sqlite3.h"
#include "Core\Object.h"

namespace Timey {

	enum SQLType {
		integer = SQLITE_INTEGER,
		real = SQLITE_FLOAT, // 64-bit.
		text = SQLITE_TEXT,
		blob = SQLITE_BLOB,
		null = SQLITE_NULL,
		undef = 0 // Undefined.

	};

	struct SqliteEntry {

		union EntryData {
			int64_t i; // SQLite Integer Type
			double d;  // SQLite Real Type
			char* c;   // SQLite Text Type
			void* v;   // SQLite BLOB Type
		};

		SQLType type = SQLType::undef;
		std::size_t size = 0;
		EntryData data;

		SqliteEntry() {
			data.v = nullptr;
			size = 0;
			type = SQLType::null;
		};

		template <typename T>
		SqliteEntry(T Data) {

			// TODO: Using C++20 feature. Should be more backward compatible. 
			static constexpr bool isInteger = std::numeric_limits<std::remove_cvref_t<T>>::is_integer;
			static constexpr bool isReal = std::is_floating_point_v<std::remove_cvref_t<T>>;
			static constexpr bool isText = std::is_same_v<std::remove_cvref_t<T>, std::string>;

			static_assert(isInteger || isReal || isText);


			if constexpr (isInteger) {
				data.i = (int64_t)Data;
				type = SQLType::integer;
				size = sizeof(int64_t);
			}
			else if constexpr (isReal) {
				data.d = (double)Data;
				type = SQLType::real;
				size = sizeof(double);
			}
			else if constexpr (isText) {

				const char* src = Data.c_str();
				data.c = (char*)malloc(Data.size() + 1);
				strcpy(data.c, src);

				type = SQLType::text;
				size = Data.size() + 1;
			}

		};

		template<typename T>
		SqliteEntry(T Data, std::size_t sz) {

			static constexpr bool isText = std::is_same_v<std::remove_cvref_t<T>, char const*> || std::is_same_v<std::remove_cvref_t<T>, char*>;
			static constexpr bool isBlob = std::is_pointer_v<std::remove_cvref_t<T>> && (!isText);

			static_assert(isText || isBlob);


			if constexpr (isText) {

				const char* src = Data;
				data.c = new char[sz];
				strcpy(data.c, src);
				type = SQLType::text;
				size = sz;

			}
			else if constexpr (isBlob) {
				const void* src = Data;
				data.v = new char[sz];
				memcpy(data.v, src, sz);
				type = SQLType::blob;
				size = sz;

			};

		};

		SqliteEntry(const SqliteEntry& Other)
			:type{ Other.type }, size{ Other.size }, data{ Other.data }
		{
			if ((type == SQLType::text) || (type == SQLType::blob)) {
				data.v = malloc(Other.size);
				memcpy(data.v, Other.data.v, size);
			};
		};


		SqliteEntry(SqliteEntry&& Other) noexcept
			:type{ Other.type }, size{ Other.size }, data{ Other.data }
		{
			if ((type == SQLType::text) || (type == SQLType::blob)) {
				Other.data.v = nullptr;
				Other.type = SQLType::undef;
			};
		};

		SqliteEntry& operator = (const SqliteEntry& Other)
		{
			type = Other.type;
			size = Other.size;
			data = Other.data;
			if ((type == SQLType::text) || (type == SQLType::blob))
			{
				data.v = malloc(Other.size);
				memcpy(data.v, Other.data.v, size);
			};

			return *this;
		};

		SqliteEntry& operator = (SqliteEntry&& Other) noexcept
		{
			type = Other.type;
			size = Other.size;
			data = Other.data;

			Other.type = SQLType::undef;
			Other.data.v = nullptr;

			return *this;
		};


		~SqliteEntry() {

			if (type == SQLType::text || type == SQLType::blob) {
				delete[] data.v;
			};
		};




		operator int64_t () { return data.i; };
		operator long int() { return data.i; };


		operator int() { return static_cast<int>(data.i); };
		operator uint32_t() { return static_cast<uint32_t>(data.i); };

		operator double() { return data.d; };
		operator float() { return static_cast<float>(data.d); };
		operator char const* () { return static_cast<char const*>(data.c); };
		operator char* () { return data.c; }
		operator void* () { return data.v; };


	};

	class SQLiteTable;
	class SqliteDb;
	using SqliteRow = std::vector<SqliteEntry>;
	using SqliteColumn = std::vector<SqliteEntry>;

	using SqliteRowMap = std::unordered_map<uint32_t, Ref<SqliteRow>>;

	class SqliteTable {

		friend class SqliteQuery;
	public:

		inline int getColumnCount() const { return colCount; };
		inline uint32_t getCurrentRow() const { return currentRow; };

		Ref<SqliteRow> getCurrentRow();
		int nextRow();
		void resetRow();
		inline bool isEmpty() { return is_empty; };
		std::string toString();
		Ref<SqliteColumn> getColumn(std::size_t col);
		Ref<SqliteColumn> operator [] (std::size_t idx) { return getColumn(idx); };



	private:
		// This class can only be constructed by exec() in SqliteQuery. 
		SqliteTable(sqlite3_stmt* stmt)
			:stmt(stmt) {
	
			int ok = sqlite3_step(stmt);

			if ((ok != SQLITE_ROW) && (ok != SQLITE_DONE)) {
				TIMEY_CORE_ERROR("Execute query failure. ({0})", ok);
				return;
			};

			is_empty = (ok == SQLITE_DONE) ? true : false;
			currentRow = 1;
			colCount = sqlite3_column_count(stmt);
		};

		int64_t getEntryInt(uint32_t col);
		double getEntryReal(uint32_t col);
		const char* getEntryText(uint32_t col, std::size_t& sz);
		const void* getEntryBlob(uint32_t col, std::size_t& sz);
		void setEmpty(bool e) { is_empty = e; };

		static SQLType getTypeFromNativeType(int type);
		SqliteRowMap cache;
		bool is_empty;


	private:

		sqlite3_stmt* stmt;
		uint32_t currentRow = 0;
		uint32_t colCount;
	};




	class SqliteQuery
	{
		friend class SqliteTable;
	public:
		SqliteQuery(const std::string& queryStr)
			:query(queryStr) {};

		~SqliteQuery() {};

		SqliteQuery() = default;
		
		SqliteQuery(const SqliteQuery& other) = default;
		SqliteQuery( SqliteQuery&& other) noexcept = default;
		SqliteQuery& operator = (const SqliteQuery& other) = default;
		SqliteQuery& operator = (SqliteQuery&& other) noexcept = default;

		int compile(const SqliteDb& db); // Binding db will make the qurey status become prepared.
		void detach();

		int bindColumnInteger(uint32_t idx, int value);
		int bindColumnReal(uint32_t idx, double value);
		int bindColumnText(uint32_t idx, const char* value);
		int bindColumnText(uint32_t idx, const std::string& value);
		int bindColumnNull(uint32_t idx);
		int bindColumnBlob(uint32_t idx, const void* data, uint32_t size);
		int unbindAllColumns();

		Ref<SqliteTable> exec();

		inline bool isPrepared() { return prepared; };
		inline const SqliteDb* getDbHandle() { return bindedDb; };

	private:
		void bdErrorMsg(int errcode);
	private:
		bool prepared = false;
		const SqliteDb* bindedDb = nullptr;
		sqlite3_stmt* stmt = nullptr;
		std::string query;
		uint32_t rowCount = 0;

	};

	class SqliteDb {

	public:
		SqliteDb(const std::string& filePath)
			:dbPath(filePath)
		{
			initDb(filePath);
		};

		~SqliteDb() {
			closeDb();
		};

		inline sqlite3* getRawDbHanle() const { return sqliteHandle; };

	private:
		void initDb(const std::string& filePath);
		void closeDb();

		sqlite3* sqliteHandle;
		std::string dbPath;
	};

	using QueryCache = std::unordered_map<std::string, SqliteQuery>;

	template <typename DataType>
	class DataBase : public SqliteDb
	{

	public:
		DataBase(const std::string& filename)
			: SqliteDb{ filename }
		{

		};

		virtual int insertData(const DataType& data) = 0;
		virtual int updateData(const DataType& data) = 0;
		virtual int deleteData(uint32_t id) = 0;
		virtual Ref<DataType> fetchData(uint32_t id) = 0;



		inline Ref<SqliteQuery> getQuery(const std::string& name) { return CreateRef<SqliteQuery>(queryCache[name]); };
		
	protected:
		void installQuery(const std::string& name, const SqliteQuery& query);
		Ref<SqliteTable> execQuery(const std::string& name);
	private:
		QueryCache queryCache;

	};



	class ProjectDb : public DataBase<Project> {

	public:

		ProjectDb(const std::string& filename)
			: DataBase<Project>{ filename }
		{
			initProjectDb();
		};

		virtual int insertData(const Project& data) override;
		virtual int updateData(const Project& data) override;
		virtual int deleteData(uint32_t id) override;
		virtual Ref<Project> fetchData(uint32_t id) override;
	
	private:
		void initProjectDb();
	};


	void print_res();
	

};
