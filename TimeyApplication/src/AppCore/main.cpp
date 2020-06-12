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

	Timey::Date day_1[4] = {
		{2020, 6, 12, 5, 35, 26},
		{2020, 6, 12, 6, 37, 4},
		{2020, 6, 12, 8, 30, 0},
		{2020, 6, 12, 9, 41, 58}
	};

	
	Timey::Session session_1 = { "Timey", "Working on Timey APP.", day_1[0], day_1[1], 3056.4f };
	Timey::Session session_2 = { "Timey", "Planning on Timey APP.", day_1[2], day_1[3], 3444.4f };

	Timey::SessionDataBase* database = new Timey::SessionDataBase("../databases/test.db");

	database->AddSession(session_1);
	database->AddSession(session_2);

};

