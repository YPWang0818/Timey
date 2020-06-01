#pragma once
#include"timey_pch.h"
#include"sqlite3.h"

namespace Timey {

	#define _COSTOM_BINDING_ASSERT(ok) 		if ((ok != SQLITE_OK)) {\
											TIMEY_CORE_ERROR("[Sqlite3] Binding Failure {0}", ok);\
											TIMEY_CORE_ASSERT(false, "");}

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
			_COSTOM_BINDING_ASSERT(ok);
		};

		template<>
		void Bind<double>(int idx, double param) {

			int ok = sqlite3_bind_double(_stmt, idx, param);
			_COSTOM_BINDING_ASSERT(ok);
		};

		template<>
		void Bind<const char*>(int idx, const char* param) {
			int ok = sqlite3_bind_text(_stmt, idx, param, -1, nullptr);
			_COSTOM_BINDING_ASSERT(ok);
		};



		void Unbind();
	private:
		sqlite3_stmt* _stmt;
		CoreDataBase* _database;

	};


};