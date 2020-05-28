#pragma once
#include"timey_pch.h"
#include"sqlite3.h"

namespace Timey {

	class CoreDataBase;

	class Query {

	public:
		Query(const std::string& name, const std::string& query, CoreDataBase* db);
		
		inline std::string getName() const { return _name; };
		inline std::string getQueryCommand() const { return _query; };

		template <class T>
		void Bind(int idx, T* param) {


		};

		void UnBind();
	private:
		sqlite3_stmt* _stmt;
		CoreDataBase* _database;
		std::string _name;
		std::string _query; // for debug purpose only;

	};


};