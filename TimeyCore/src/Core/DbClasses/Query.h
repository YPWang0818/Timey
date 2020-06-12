#pragma once
#include"timey_pch.h"
#include"sqlite3.h"
#include "QueryResult.h"

namespace Timey {


										
	

	class CoreDataBase;

	class Query {

	public:
		Query(const std::string& query, CoreDataBase* db);
		~Query();
		

		template <class T>
		void Bind(int idx, T param) {

			TIMEY_CORE_ASSERT(false, "Unsupported type.");
		};

		// tamplate specialization.
		template<>
		void Bind<int>(int idx, int param) {
			
			int ok = sqlite3_bind_int(_stmt, idx, param);
			_binding_errmsg(ok);
		};

		template<>
		void Bind<double>(int idx, double param) {

			int ok = sqlite3_bind_double(_stmt, idx, param);
			_binding_errmsg(ok);
		};

		template<>
		void Bind<const std::string&>(int idx, const std::string& param) {
			int ok = sqlite3_bind_text(_stmt, idx, param.c_str(), -1, nullptr);
			_binding_errmsg(ok);
		};

		void Unbind();
		QueryResult* Exec();
		void ExecOnceNoRes();

	private:

		void _binding_errmsg(int ok);

		sqlite3_stmt* _stmt;
		CoreDataBase* _database;

	};


};