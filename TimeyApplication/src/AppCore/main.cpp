#include "timey_pch.h"
#include "Application.h"
#include "TimeyCore.h"

// Test for basic functionality.

static Timey::Date dates[4] = {
		{2020, 6, 12, 5, 35, 26},
		{2020, 6, 12, 6, 37, 4},
		{2020, 6, 12, 8, 30, 0},
		{2020, 6, 12, 9, 41, 58}
};

static Timey::Color c_red = { 0.8f, 0.1f, 0.1f, 1.0f };
static Timey::Color c_blue = { 0.0f, 0.0f, 0.1f, 1.0f };

static std::string test_db_path = "../databases/test.db";

Timey::SessionDataBase* session_db;
Timey::TagDataBase* tag_db;
Timey::ProjectDataBase* project_db;


static int init_test_db() {



	Timey::Session session_1 = { "Witing code", "Working on Timey APP.", dates[0], dates[1], 3056.4f, 0 };
	Timey::Tag tag_1 = { "Coding", c_red, 0 };
	Timey::Project project_1 = { "Timey", "A time tracking app.", c_blue, 0 };


	session_db->AddSession(session_1);
	tag_db->AddTag(tag_1);
	project_db->AddProject(project_1);


	return 1;
};

static int test_basic_rw(bool test) {
	// Testing basic reading and writing to the database.
	if(!test) return 0;

	TIMEY_WARN("***********  BASIC READ WIRTE TEST ***********\n\n");

	Timey::Session session_2 = { "Timey2", "Planning on Timey APP.", dates[2], dates[3], 3444.4f, 13 };
	Timey::Tag tag_2 = { "High level planning", c_blue, 21 };
	Timey::Project project_2 = { "Untited game engine", "Mini game engine", c_red, 6 };

	session_db->AddSession(session_2);
	tag_db->AddTag(tag_2);
	project_db->AddProject(project_2);

	TIMEY_INFO("----- WRITING OBJECT\n\n");
	std::shared_ptr<Timey::Session> s1 = session_db->FetchSession(1);
	std::shared_ptr<Timey::Session> s2 = session_db->FetchSession(2);
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

	TIMEY_INFO("----- DELETEING OBJECT\n\n");

	session_db->DeleteSession(*s2);
	tag_db->DeleteTag(*t2);
	project_db->DeleteProject(*p2);

	std::shared_ptr<Timey::Session> s3 = session_db->FetchSession(1);
	std::shared_ptr<Timey::Session> s4 = session_db->FetchSession(2);

	std::shared_ptr<Timey::Tag> t3 = tag_db->FetchTag(1);
	std::shared_ptr<Timey::Tag> t4 = tag_db->FetchTag(2);

	std::shared_ptr<Timey::Project> p3 = project_db->FetchProject(1);
	std::shared_ptr<Timey::Project> p4 = project_db->FetchProject(2);

	TIMEY_TRACE("[Session 2]\n{0}", *s3);
	TIMEY_TRACE("[Tag 2]\n{0}", *t3);
	TIMEY_TRACE("[Project 2]\n{0}", *p3);

	return 1;
};

static int test_object_group_update(bool test) {
	// Test updating tag group and project group.
	if (!test) return 0;

	TIMEY_WARN("***********  GROUP OBJ UPDATE TEST ***********\n\n");

	Timey::TagGroup tag_gp = {69, "Correction tags", 0};
	Timey::ProjectGroup project_gp = {420, "Academic", 0};

	std::shared_ptr<Timey::Tag> t1 = tag_db->FetchTag(1);
	std::shared_ptr<Timey::Project> p1 = project_db->FetchProject(1);

	TIMEY_INFO("----- BEFORE UPDATE\n\n");

	TIMEY_TRACE("[Tag 1]\n{0}", *t1);
	TIMEY_TRACE("[Project 1]\n{0}", *p1);

	tag_db->UpdateTagsGroup(*t1, tag_gp);
	project_db->UpdateProjectsGroup(*p1, project_gp);

	TIMEY_INFO("----- AFTER UPDATE\n\n");

	std::shared_ptr<Timey::Tag> t2 = tag_db->FetchTag(1);
	std::shared_ptr<Timey::Project> p2 = project_db->FetchProject(1);

	TIMEY_TRACE("[Tag 1]\n{0}", *t2);
	TIMEY_TRACE("[Project 1]\n{0}", *p2);
	
	bool res = (t2->tag_group_id == tag_gp.ID) && (p2->project_group_id == project_gp.ID);
	return res;

}

int main(int argc, char** argv) {

	Timey::Log::Init();

	Timey::Application* app;
	app = Timey::CreateApp();

	//app->Run();


	Timey::FileManager& file_manager = Timey::FileManager::getFileManager();

	if (file_manager.FindIfFileExists(test_db_path)){
		file_manager.RemoveFile(test_db_path);
	};

	session_db = new Timey::SessionDataBase(test_db_path);
	tag_db = new Timey::TagDataBase(test_db_path);
	project_db = new Timey::ProjectDataBase(test_db_path);

	init_test_db();
	test_basic_rw(false);
	bool res = test_object_group_update(true);

}

