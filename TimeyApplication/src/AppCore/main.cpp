#include "timey_pch.h"
#include "Application.h"
#include "TimeyCore.h"

template<class T>
void _print_column(const std::vector<T>& data) 
{
	for (auto it = data.begin(); it != data.end(); ++it) {
		TIMEY_TRACE("{0}", *it);
	}
}

void _dump_all_info(Timey::QueryResult* query_res) {

	int col_num = query_res->getColumnCount();

	if (query_res->isEmpty()) {
		TIMEY_INFO("Empty Query");
		return;
	}

	for ( int i = 1; i <= col_num; i++) {

		TIMEY_INFO("---- column {0} -----", i );

		switch(query_res->getColumnType(i))
		{
		case Timey::SQLiteType::integer:   _print_column<int>(query_res->getColumn<int>(i)); break;
		case Timey::SQLiteType::real:	   _print_column<double>(query_res->getColumn<double>(i)); break;
		case Timey::SQLiteType::text:     _print_column<std::string>(query_res->getColumn<std::string>(i)); break;
		case Timey::SQLiteType::blob:	  TIMEY_INFO("blob unsupported."); break;
		case Timey::SQLiteType::null:	  TIMEY_INFO("NULL column"); break;

		}

	}

};

int main(int argc, char** argv) {

	Timey::Log::Init();

	Timey::Application* app;
	app = Timey::CreateApp();

	//app->Run();

	const std::string _create_table = R"(		
	CREATE TABLE contacts (
	contact_id INTEGER PRIMARY KEY,
	first_name TEXT NOT NULL,
	last_name TEXT NOT NULL,
	email TEXT NOT NULL UNIQUE,
	phone TEXT NOT NULL UNIQUE);
		)";

	const std::string _find_schema = R"(
	PRAGMA table_info(contacts)
		)";

	const std::string _insert_row = R"(
	INSERT INTO contacts (first_name ,last_name , email, phone)
	VALUES( 'yu-ping',	'wang' , 'i8181995@yahoo.com', 6319771679);
		)";
	

	const std::string _show_all = R"(
		SELECT * FROM contacts;
		)";

	Timey::CoreDataBase* database = new Timey::CoreDataBase("../databases/example.db");
	Timey::Query* query = new Timey::Query(_show_all, database);
	Timey::QueryResult* qres = query->Exec();

	_dump_all_info(qres);

	delete qres;
	delete query;
	delete database;

};

