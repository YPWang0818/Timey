#include "timey_pch.h"
#include "Application.h"
#include "TimeyCore.h"



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

	
	Timey::Session session_1 = { "Timey", "Working on Timey APP.", day_1[0], day_1[1], 3056.4f, 1 };
	Timey::Session session_2 = { "Timey", "Planning on Timey APP.", day_1[2], day_1[3], 3444.4f, 2 };

	Timey::SessionDataBase* database = new Timey::SessionDataBase("../databases/test.db");

	database->AddSession(session_1);
	database->AddSession(session_2);

	std::shared_ptr<Timey::Session> s1 = database->FetchSession(1);
	std::shared_ptr<Timey::Session> s2 = database->FetchSession(2);
};

