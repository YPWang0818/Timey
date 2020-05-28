#pragma once
#include "timey_pch.h"
#include "sqlite3.h"

namespace Timey {

	class Query;

	class CoreDataBase {

	public:

		CoreDataBase(const std::string& file_name);
		~CoreDataBase();

		inline sqlite3* getSqliteDb() const { return _db; };
		
	protected:
		void AddQuery(const std::string& name, Query& query);
		void CreateQuery(const std::string& name, const std::string& stmt);
		void RemoveQuery(const std::string& name);


	private:

		sqlite3* _db;
		std::string  _file_name;
		std::unordered_map<std::string, Query*> _query_cache;

	};
}