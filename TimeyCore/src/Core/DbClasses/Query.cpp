#include "timey_pch.h"
#include "Query.h"
#include "CoreDataBase.h"

namespace Timey {



	Query::Query(const std::string& name, const std::string& query, CoreDataBase* db)
		:_name(name),
		_query(query),
		_database(db)
	{
		sqlite3_prepare_v2(_database->getSqliteDb(), query.c_str() , -1, &_stmt, nullptr);
	}

	void Query::UnBind()
	{

	}

}