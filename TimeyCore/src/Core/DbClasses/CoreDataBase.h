#pragma once
#include "timey_pch.h"
#include "sqlite3.h"
#include "Query.h"

namespace Timey {

	
	class CoreDataBase {

	public:

		CoreDataBase(const std::string& file_name);
		~CoreDataBase();
		inline sqlite3* getSqliteDb() const { return _db; };

	protected:
		void AddQuery(const std::string& name, std::shared_ptr<Query> query);
		void RemoveQuery(const std::string& name);
		inline std::unordered_map<std::string, std::shared_ptr<Query> > getQueries() { return _query_cache; };

	private:
		void _create_data_base();

		sqlite3* _db;
		std::string  _file_name;
		std::unordered_map<std::string, std::shared_ptr<Query> > _query_cache;

	};
}
