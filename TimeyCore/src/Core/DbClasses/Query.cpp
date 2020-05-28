#include "timey_pch.h"
#include "Query.h"

namespace Timey {



	Query::Query(const std::string& name, const std::string& query, CoreDataBase* db)
		:_name(name),
		_query(query),
		_database(db)
	{
		
	
		// initialize the stmt here
	}

	void Query::UnBind()
	{

	}

}