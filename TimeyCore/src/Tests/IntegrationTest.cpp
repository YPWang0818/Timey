#include "timey_pch.h"
#include "IntegrationTest.h"



namespace Timey {

	IntegrationTests* IntegrationTests::m_instance = nullptr;

	IntegrationTests::IntegrationTests()
	{
		
			m_integrationTest.push_back(CreateScope<DataBaseTest>("Project DataBase Tests",
				"../databases/TestDB.db"));
	
		/*
		m_integrationTest.push_back(CreateScope<SqltileDataBaseLockTest>("Lock database tests.",
			"../databases/LockTest.db"));		
		*/
	}

	IntegrationTests::~IntegrationTests()
	{
	}

	void IntegrationTests::Run()
	{	
		auto& integrationTests = IntegrationTests::getTests().getTestsInternals();
		for (auto& t : integrationTests) {
			t->setup();
			t->run();
			t->teardown();
		};

	}

	int IntegrationTestSuite::randNumber(int max, int min)
	{
		if (max <= min) return min;

		static std::random_device randDev;
		static std::mt19937 Gen(randDev());
		static std::uniform_int_distribution<std::size_t> dist;

		return min + (dist(Gen) % (max - min + 1));
	};

	float IntegrationTestSuite::randReal(float max, float min)
	{
		if (max <= min) return min;
		static std::random_device randDev;
		static std::mt19937 Gen(randDev());

		std::uniform_real_distribution dist(min, max);
		return dist(Gen);
	};

	std::string IntegrationTestSuite::randString(std::size_t len)
	{
		std::string result;
		result.reserve(len);

		for (int i = 0; i < len; i++) {
			int charType = randNumber(10);
			if (charType == 0) {
				// push a space character.
				result.push_back(' ');
				continue;
			}
			else if (charType  < 5) {
				// push a random upper case character.
				result.push_back('A' + randNumber(25));
				continue;
			}
			else {
				// push a random lower case character.
				result.push_back('a' + randNumber(25));
				continue;
			}
		}
		return result;
	};

	void DataBaseTest::run() {

		test_Projects_DataBase();
		test_Sessions_DataBase();
		test_Tags_DataBase();
		test_ProjectGroup_DataBase();
		test_TagGroup_DataBase();

	};

	void IntegrationTestSuite::setup() {
		TIMEY_TRACE("Start test suite {} ...", m_testName);
	};

	void IntegrationTestSuite::teardown() {
		TIMEY_TRACE(" End test suite {}", m_testName);
	};

	Ref<Project>  DataBaseTest::genProjects() {

		Ref<Project> proj = CreateRef<Project>();

		proj->color = { randReal(), randReal(), randReal(), randReal() };
		proj->name = randString(20);
		proj->discription = randString(128);
		proj->project_group_id = randNumber(1024);

		return proj;
	}

	Ref<Session> DataBaseTest::genSessions()
	{
		Ref<Session> session = CreateRef<Session>();
		session->duration = randNumber(1024);
		session->start_time = randNumber(UINT32_MAX);
		session->end_time =  randNumber(UINT32_MAX);
		session->name = randString(20);
		session->discription = randString(128);
		session->project_id = randNumber(65536);

		return session;
	}

	Ref<Tag> DataBaseTest::genTags()
	{
		Ref<Tag> tag = CreateRef<Tag>();

		tag->color = { randReal(), randReal(), randReal(), randReal() };
		tag->name = randString(10);
		tag->tag_group_id = randNumber(1024);

		return tag;
	}

	Ref<ProjectGroup> DataBaseTest::genProjectGroups()
	{
		Ref<ProjectGroup> pggroup = CreateRef<ProjectGroup>();
		pggroup->name = randString(10);
		pggroup->parentID = randNumber(1024);

		return  pggroup;
	}

	Ref<TagGroup> DataBaseTest::genTagGroups()
	{
		Ref<TagGroup> taggroup = CreateRef<TagGroup>();
		taggroup->name = randString(10);
		taggroup->parentID = randNumber(1024);

		return taggroup;
	}

	void DataBaseTest::copyProjectContent(Ref<Project> src, Ref<Project> dest)
	{
		dest->color = src->color;
		dest->name = src->name;
		dest->discription = src->discription;
		dest->project_group_id = src->project_group_id;
	}

	void DataBaseTest::copySessionContent(const Ref<Session> src, Ref<Session> dest)
	{
			dest->duration = src->duration;
			dest->start_time= src->start_time;
			dest->end_time = src->end_time;
			dest->name = src->name;
			dest->discription = src->discription;
			dest->project_id = src->project_id;
			dest->tag_list = src->tag_list;

	}

	void DataBaseTest::copyTagContent(const Ref<Tag> src, Ref<Tag> dest)
	{
		dest->color = src->color;
		dest->name = src->name;
		dest->tag_group_id = src->tag_group_id;
	}

	void DataBaseTest::copyProjectGroupContent(const Ref<ProjectGroup> src, Ref<ProjectGroup> dest)
	{
		dest->name = src->name;
		dest->parentID = src->parentID;
	}

	void DataBaseTest::copyTagGroupContent(const Ref<TagGroup> src, Ref<TagGroup> dest)
	{
		dest->name = src->name;
		dest->parentID = src->parentID;
	}

	void DataBaseTest::test_Projects_DataBase()
	{
		TIMEY_INFO("**************** Test Project DataBase ****************");

		std::vector<Ref<Project>> ramdom_projects;
		std::size_t projects_num = 20;

		//Generating data
		TIMEY_TRACE("Generating {} ramdom projects.", projects_num);
		for (int i = 0; i < projects_num; i++) {
			ramdom_projects.push_back(genProjects());
		}

		// Inserting data into database
		TIMEY_TRACE("Inserting ramdom projects into {}.", m_dbpath);
		for (auto p : ramdom_projects) {
			projdb->insertData(*p);
		};

		
		TIMEY_TRACE("Fetching each project by ID and compare them to the expected value.");
		// Fetch all of them
		bool fetch_test = true;
		for (int i = 1; i <= projects_num; i++) {
			Ref<Project> p1 = projdb->fetchData(i);
			Ref<Project> p2 = ramdom_projects[i - 1];
			fetch_test = fetch_test && (*p1 == *p2);
		};

		if (fetch_test) {
			TIMEY_INFO("Fetch test passed.");
		}
		else {
			TIMEY_ERROR("Fetch test failed.");
		};

		
		ramdom_projects.clear();

		TIMEY_TRACE("Generating new {} projects and updating the database.", projects_num);
		// Updating test.
		for (int i = 1; i <= projects_num; i++) {
			Ref<Project> p1 = projdb->fetchData(i);
			Ref<Project> p2 = genProjects();
			copyProjectContent(p2, p1);
			projdb->updateData(*p1);
			ramdom_projects.push_back(p1);

		};

		
		TIMEY_TRACE("Comparing the update value with the database.");
		// Fetch and compare the updated data.
		bool update_test = true;
		for (int i = 1; i <= projects_num; i++) {
			Ref<Project> p1 = projdb->fetchData(i);
			Ref<Project> p2 = ramdom_projects[i - 1];
			update_test = update_test && (*p1 == *p2);
		};

		if (update_test) {
			TIMEY_INFO("Update test passed.");
		}
		else {
			TIMEY_ERROR("Update test failed.");
		};


		TIMEY_TRACE("Deleteing all data");
		for (int i = 1; i <= projects_num; i++) {
			projdb->deleteData(i);
		};


	
		TIMEY_TRACE("Quering the database to see if the database is empty.");
		SqliteQuery test_if_the_projects_table_is_empty = SqliteQuery(
			" SELECT COUNT(*) FROM projects; "
		);

		
		test_if_the_projects_table_is_empty.compile(*projdb);
		Ref<SqliteTable> result = test_if_the_projects_table_is_empty.exec();
	
		int count = 1; // Initialized count to a wrong value.
		count = (int)( *(result->getCurrentRow()) )[0];
		if (!count) {
			TIMEY_INFO("Update test passed.");
		}
		else {
			TIMEY_ERROR("Update test failed, there are {} entries in the database", count);
		};

		test_if_the_projects_table_is_empty.detach();
	

	}

	void DataBaseTest::test_Sessions_DataBase()
	{

		TIMEY_INFO("**************** Test Session DataBase ****************");

		std::vector<Ref<Session>> ramdom_sessions;
		std::size_t sessions_num = 20;

		//Generating data
		TIMEY_TRACE("Generating {} ramdom sessions.", sessions_num);
		for (int i = 0; i < sessions_num; i++) {
			ramdom_sessions.push_back(genSessions());
		}

		// Inserting data into database
		TIMEY_TRACE("Inserting ramdom sessions into {}.", m_dbpath);
		for (auto s : ramdom_sessions) {
			sessiondb->insertData(*s);
		};

		
		TIMEY_TRACE("Fetching each sessions by ID and compare them to the expected value.");
		// Fetch all of them
		bool fetch_test = true;
		for (int i = 1; i <= sessions_num; i++) {
			Ref<Session> s1 = sessiondb->fetchData(i);
			Ref<Session> s2 = ramdom_sessions[i - 1];
			fetch_test = fetch_test && (*s1 == *s2);
		};


		if (fetch_test) {
			TIMEY_INFO("Fetch test passed.");
		}
		else {
			TIMEY_ERROR("Fetch test failed.");
		};

		ramdom_sessions.clear();

		TIMEY_TRACE("Generating new {} sessions and updating the database.", sessions_num);
		// Updating test.
		for (int i = 1; i <= sessions_num; i++) {
			Ref<Session> s1 = sessiondb->fetchData(i);
			Ref<Session> s2 = genSessions();
			copySessionContent(s2, s1);
			sessiondb->updateData(*s1);
			ramdom_sessions.push_back(s1);

		};

		TIMEY_TRACE("Comparing the update value with the database.");
		// Fetch and compare the updated data.
		bool update_test = true;
		for (int i = 1; i <= sessions_num; i++) {
			Ref<Session> s1 = sessiondb->fetchData(i);
			Ref<Session> s2 = ramdom_sessions[i - 1];
			update_test = update_test && (*s1 == *s2);
		};

		if (update_test) {
			TIMEY_INFO("Update test passed.");
		}
		else {
			TIMEY_ERROR("Update test failed.");
		};

		TIMEY_TRACE("Deleteing all data");
		for (int i = 1; i <= sessions_num; i++) {
			sessiondb->deleteData(i);
		};


		TIMEY_TRACE("Quering the database to see if the database is empty.");
		SqliteQuery test_if_the_sessions_table_is_empty = SqliteQuery(
			" SELECT COUNT(*) FROM sessions; "
		);

		test_if_the_sessions_table_is_empty.compile(*sessiondb);
		Ref<SqliteTable> result = test_if_the_sessions_table_is_empty.exec();

		int count = 1; // Initialized count to a wrong value.
		count = (int)(*(result->getCurrentRow()))[0];
		if (!count) {
			TIMEY_INFO("Update test passed.");
		}
		else {
			TIMEY_ERROR("Update test failed, there are {} entries in the database", count);
		};
	
		test_if_the_sessions_table_is_empty.detach();

	}

	void DataBaseTest::test_Tags_DataBase()
	{

		TIMEY_INFO("**************** Test Tags DataBase ****************");

		std::vector<Ref<Tag>> ramdom_tags;
		std::size_t tags_num = 100;

		//Generating data
		TIMEY_TRACE("Generating {} ramdom tags.", tags_num);
		for (int i = 0; i < tags_num; i++) {
			ramdom_tags.push_back(genTags());
		}

		// Inserting data into database
		TIMEY_TRACE("Inserting ramdom tags into {}.", m_dbpath);
		for (auto s : ramdom_tags) {
			tagdb->insertData(*s);
		};


		TIMEY_TRACE("Fetching each tags by ID and compare them to the expected value.");
		// Fetch all of them
		bool fetch_test = true;
		for (int i = 1; i <= tags_num; i++) {
			Ref<Tag> t1 = tagdb->fetchData(i);
			Ref<Tag> t2 = ramdom_tags[i - 1];
			fetch_test = fetch_test && (*t1 == *t2);
		};


		if (fetch_test) {
			TIMEY_INFO("Fetch test passed.");
		}
		else {
			TIMEY_ERROR("Fetch test failed.");
		};

		ramdom_tags.clear();

		TIMEY_TRACE("Generating new {} tags and updating the database.", tags_num);
		// Updating test.
		for (int i = 1; i <= tags_num; i++) {
			Ref<Tag> t1 = tagdb->fetchData(i);
			Ref<Tag> t2 = genTags();
			copyTagContent(t2, t1);
			tagdb->updateData(*t1);
			ramdom_tags.push_back(t1);

		};

		TIMEY_TRACE("Comparing the update value with the database.");
		// Fetch and compare the updated data.
		bool update_test = true;
		for (int i = 1; i <= tags_num; i++) {
			Ref<Tag> t1 = tagdb->fetchData(i);
			Ref<Tag> t2 = ramdom_tags[i - 1];
			update_test = update_test && (*t1 == *t2);
		};

		if (update_test) {
			TIMEY_INFO("Update test passed.");
		}
		else {
			TIMEY_ERROR("Update test failed.");
		};

		TIMEY_TRACE("Deleteing all data");
		for (int i = 1; i <= tags_num; i++) {
			tagdb->deleteData(i);
		};


		TIMEY_TRACE("Quering the database to see if the database is empty.");
		SqliteQuery test_if_the_tags_table_is_empty = SqliteQuery(
			" SELECT COUNT(*) FROM tags; "
		);

		test_if_the_tags_table_is_empty.compile(*tagdb);
		Ref<SqliteTable> result = test_if_the_tags_table_is_empty.exec();

		int count = 1; // Initialized count to a wrong value.
		count = (int)(*(result->getCurrentRow()))[0];
		if (!count) {
			TIMEY_INFO("Update test passed.");
		}
		else {
			TIMEY_ERROR("Update test failed, there are {} entries in the database", count);
		};

		test_if_the_tags_table_is_empty.detach();
	}

	void DataBaseTest::test_ProjectGroup_DataBase()
	{
		TIMEY_INFO("**************** Test Project Group DataBase ****************");

		std::vector<Ref<ProjectGroup>> ramdom_projectgps;
		std::size_t projectgp_num = 100;

		//Generating data
		TIMEY_TRACE("Generating {} ramdom project group.", projectgp_num);
		for (int i = 0; i < projectgp_num; i++) {
			ramdom_projectgps.push_back(genProjectGroups());
		}

		// Inserting data into database
		TIMEY_TRACE("Inserting ramdom project groups into {}.", m_dbpath);
		for (auto s : ramdom_projectgps) {
			projgpdb->insertData(*s);
		};


		TIMEY_TRACE("Fetching each project groups by ID and compare them to the expected value.");
		// Fetch all of them
		bool fetch_test = true;
		for (int i = 1; i <= projectgp_num; i++) {
			Ref<ProjectGroup> pg1 = projgpdb->fetchData(i);
			Ref<ProjectGroup> pg2 = ramdom_projectgps[i - 1];
			fetch_test = fetch_test && (*pg1 == *pg2);
		};


		if (fetch_test) {
			TIMEY_INFO("Fetch test passed.");
		}
		else {
			TIMEY_ERROR("Fetch test failed.");
		};

		ramdom_projectgps.clear();

		TIMEY_TRACE("Generating new {} project groups and updating the database.", projectgp_num);
		// Updating test.
		for (int i = 1; i <= projectgp_num; i++) {
			Ref<ProjectGroup> t1 = projgpdb->fetchData(i);
			Ref<ProjectGroup> t2 = genProjectGroups();
			copyProjectGroupContent(t2, t1);
			projgpdb->updateData(*t1);
			ramdom_projectgps.push_back(t1);

		};

		TIMEY_TRACE("Comparing the update value with the database.");
		// Fetch and compare the updated data.
		bool update_test = true;
		for (int i = 1; i <= projectgp_num; i++) {
			Ref<ProjectGroup> pg1 = projgpdb->fetchData(i);
			Ref<ProjectGroup> pg2 = ramdom_projectgps[i - 1];
			update_test = update_test && (*pg1 == *pg2);
		};

		if (update_test) {
			TIMEY_INFO("Update test passed.");
		}
		else {
			TIMEY_ERROR("Update test failed.");
		};

		TIMEY_TRACE("Deleteing all data");
		for (int i = 1; i <= projectgp_num; i++) {
			projgpdb->deleteData(i);
		};


		TIMEY_TRACE("Quering the database to see if the database is empty.");
		SqliteQuery test_if_the_projects_group_table_is_empty = SqliteQuery(
			" SELECT COUNT(*) FROM projects_group; "
		);

		test_if_the_projects_group_table_is_empty.compile(*projgpdb);
		Ref<SqliteTable> result = test_if_the_projects_group_table_is_empty.exec();

		int count = 1; // Initialized count to a wrong value.
		count = (int)(*(result->getCurrentRow()))[0];
		if (!count) {
			TIMEY_INFO("Update test passed.");
		}
		else {
			TIMEY_ERROR("Update test failed, there are {} entries in the database", count);
		};

		test_if_the_projects_group_table_is_empty.detach();

	}

	void DataBaseTest::test_TagGroup_DataBase()
	{
		TIMEY_INFO("**************** Test Tag Group DataBase ****************");

		std::vector<Ref<TagGroup>> ramdom_taggps;
		std::size_t taggp_num = 100;

		//Generating data
		TIMEY_TRACE("Generating {} ramdom tag groups.", taggp_num);
		for (int i = 0; i < taggp_num; i++) {
			ramdom_taggps.push_back(genTagGroups());
		}

		// Inserting data into database
		TIMEY_TRACE("Inserting ramdom tag groups into {}.", m_dbpath);
		for (auto s : ramdom_taggps) {
			taggpdb->insertData(*s);
		};


		TIMEY_TRACE("Fetching each project groups by ID and compare them to the expected value.");
		// Fetch all of them
		bool fetch_test = true;
		for (int i = 1; i <= taggp_num; i++) {
			Ref<TagGroup> tg1 = taggpdb->fetchData(i);
			Ref<TagGroup> tg2 = ramdom_taggps[i - 1];
			fetch_test = fetch_test && (*tg1 == *tg2);
		};


		if (fetch_test) {
			TIMEY_INFO("Fetch test passed.");
		}
		else {
			TIMEY_ERROR("Fetch test failed.");
		};

		ramdom_taggps.clear();

		TIMEY_TRACE("Generating new {} tag groups and updating the database.", taggp_num);
		// Updating test.
		for (int i = 1; i <= taggp_num; i++) {
			Ref<TagGroup> tg1 = taggpdb->fetchData(i);
			Ref<TagGroup> tg2 = genTagGroups();
			copyTagGroupContent(tg2, tg1);
			taggpdb->updateData(*tg1);
			ramdom_taggps.push_back(tg1);

		};

		TIMEY_TRACE("Comparing the update value with the database.");
		// Fetch and compare the updated data.
		bool update_test = true;
		for (int i = 1; i <= taggp_num; i++) {
			Ref<TagGroup> tg1 = taggpdb->fetchData(i);
			Ref<TagGroup> tg2 = ramdom_taggps[i - 1];
			update_test = update_test && (*tg1 == *tg2);
		};

		if (update_test) {
			TIMEY_INFO("Update test passed.");
		}
		else {
			TIMEY_ERROR("Update test failed.");
		};

		TIMEY_TRACE("Deleteing all data");
		for (int i = 1; i <= taggp_num; i++) {
			taggpdb->deleteData(i);
		};


		TIMEY_TRACE("Quering the database to see if the database is empty.");
		SqliteQuery test_if_the_tags_group_table_is_empty = SqliteQuery(
			" SELECT COUNT(*) FROM tags_group; "
		);

		test_if_the_tags_group_table_is_empty.compile(*taggpdb);
		Ref<SqliteTable> result = test_if_the_tags_group_table_is_empty.exec();

		int count = 1; // Initialized count to a wrong value.
		count = (int)(*(result->getCurrentRow()))[0];
		if (!count) {
			TIMEY_INFO("Update test passed.");
		}
		else {
			TIMEY_ERROR("Update test failed, there are {} entries in the database", count);
		};

		test_if_the_tags_group_table_is_empty.detach();
	}



};