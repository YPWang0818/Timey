#pragma once
#include "timey_pch.h"

namespace Timey {

	class Query;

	class CoreDataBase {

	public:

		CoreDataBase(const std::string& file_name);
		
	protected:
		void AddQuery(const std::string& name, Query& query);
		void CreateQuery(const std::string& name, const std::string& stmt);
		void RemoveQuery(const std::string& name);


	private:

		std::string  _file_name;
		std::unordered_map<Query*, std::string> _query_cache;

	};
}