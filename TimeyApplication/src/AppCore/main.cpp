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

	Timey::Color c_red = { 0.8f, 0.1f, 0.1f, 1.0f };
	Timey::Color c_blue = { 0.0f, 0.0f, 0.1f, 1.0f };


	
	Timey::FileManager& file_manager = Timey::FileManager::getFileManager();

	if (file_manager.FindIfFileExists("../databases/test.db")) {
		file_manager.RemoveFile("../databases/test.db");
	}

	Timey::Session session_1 = { "Timey", "Working on Timey APP.", day_1[0], day_1[1], 3056.4f, 3 };
	Timey::Session session_2 = { "Timey2", "Planning on Timey APP.", day_1[2], day_1[3], 3444.4f, 3 };

	Timey::Tag tag_1 = { "Coding", c_red, 0 };
	Timey::Tag tag_2 = { "High level planning", c_blue, 0};

	Timey::TagGroup tag_group_1 = { 13 , "Computer Science ", 1 };

	TIMEY_TRACE("[Tag gorup] {0}\n", tag_group_1);

	Timey::Project project_1 = { "Timey", "A time tracking app.", c_blue, 5 };
	Timey::Project project_2 = { "Untited game engine", "Mini game engine", c_red, 6};


	Timey::SessionDataBase* database = new Timey::SessionDataBase("../databases/test.db");
	Timey::TagDataBase* tag_db = new Timey::TagDataBase("../databases/test.db");
	Timey::ProjectDataBase* project_db = new Timey::ProjectDataBase("../databases/test.db");

	database->AddSession(session_1);
	database->AddSession(session_2);

	tag_db->AddTag(tag_1);
	tag_db->AddTag(tag_2);

	project_db->AddProject(project_1);
	project_db->AddProject(project_2);


	std::shared_ptr<Timey::Session> s1 = database->FetchSession(1);
	std::shared_ptr<Timey::Session> s2 = database->FetchSession(2);
	std::shared_ptr<Timey::Tag> t1 = tag_db->FetchTag(1);
	std::shared_ptr<Timey::Tag> t2 = tag_db->FetchTag(2);
	std::shared_ptr<Timey::Project> p1 = project_db->FetchProject(1);
	std::shared_ptr<Timey::Project> p2 = project_db->FetchProject(2);
	
	TIMEY_TRACE("[Session 1]\n{0}", *s1);
	TIMEY_TRACE("[Session 2]\n{0}", *s2);
	TIMEY_TRACE("[Tag 1]\n{0}", *t1);
	TIMEY_TRACE("[Tag 2]\n{0}", *t2);
	TIMEY_TRACE("[Project 1]\n{0}", *p1);
	TIMEY_TRACE("[Project 2]\n{0}", *p2);


	database->DeleteSession(*s1);
	tag_db->DeleteTag(*t1);
	project_db->DeleteProject(*p1);


	TIMEY_WARN("**************");

	std::shared_ptr<Timey::Session> s3 = database->FetchSession(1);
	std::shared_ptr<Timey::Session> s4 = database->FetchSession(2);

	std::shared_ptr<Timey::Tag> t3 = tag_db->FetchTag(1);
	std::shared_ptr<Timey::Tag> t4 = tag_db->FetchTag(2);
	tag_db->UpdateTagGroup(*t4, tag_group_1);
	t4 = tag_db->FetchTag(2);

	std::shared_ptr<Timey::Project> p3 = project_db->FetchProject(1);
	std::shared_ptr<Timey::Project> p4 = project_db->FetchProject(2);

	TIMEY_TRACE("[Session 2]\n{0}", *s4);
	TIMEY_TRACE("[Tag 2]\n{0}", *t4);
	TIMEY_TRACE("[Project 2]\n{0}", *p4);
};

