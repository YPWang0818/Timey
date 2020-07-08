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

	/*char current_dir[500];
	GetCurrentDirectoryA(300, current_dir);
	TIMEY_INFO("current direcltory: {0}", current_dir);
	*/
	Timey::FileManager& file_manager = Timey::FileManager::getFileManager();

	if (file_manager.FindIfFileExists("../databases/test.db")) {
		file_manager.RemoveFile("../databases/test.db");
	}

	Timey::Session session_1 = { "Timey", "Working on Timey APP.", day_1[0], day_1[1], 3056.4f, 3 };
	Timey::Session session_2 = { "Timey2", "Planning on Timey APP.", day_1[2], day_1[3], 3444.4f, 3 };

	Timey::SessionDataBase* database = new Timey::SessionDataBase("../databases/test.db");

	database->AddSession(session_1);
	database->AddSession(session_2);

	std::shared_ptr<Timey::Session> s1 = database->FetchSession(1);
	std::shared_ptr<Timey::Session> s2 = database->FetchSession(2);
	
	TIMEY_INFO("[Session 1]\n{0}", *s1);
	TIMEY_INFO("[Session 2]\n{0}", *s2);

	database->DeleteSession( *s1 );

	TIMEY_INFO("******* Delete session 1 *******");

	std::shared_ptr<Timey::Session> s3 = database->FetchSession(1);
	std::shared_ptr<Timey::Session> s4 = database->FetchSession(2);


	TIMEY_INFO("[Session 2]\n{0}", *s4);
};

