#include "timey_pch.h"
#include "IntegrationTest.h"
#include "DbClasses/DataBase.h"


namespace Timey {

	IntegrationTests* IntegrationTests::m_instance = nullptr;

	IntegrationTests::IntegrationTests()
	{
		m_integrationTest.push_back(CreateScope<DataBaseTest>("Project DataBase Tests",
			"../databases/TestDB.db"));
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
	};

	void IntegrationTestSuite::setup() {
		TIMEY_TRACE("Start test suite {} ...", m_testName);
	};

	void IntegrationTestSuite::teardown() {
		TIMEY_TRACE(" End test suite {}", m_testName);
	};

	Ref<Project>  DataBaseTest::genProjects() {

		Project* nwProject = new Project;
		nwProject->color = { randReal(), randReal(), randReal(), randReal() };
		nwProject->name = randString(20);
		nwProject->discription = randString(128);
		nwProject->project_group_id = randNumber(1024);

		Ref<Project> proj = CreateRef<Project>();
		proj.reset(nwProject);
		return proj;
	}

	void DataBaseTest::copyProjectContent(Ref<Project> src, Ref<Project> dest)
	{
		dest->color = src->color;
		dest->name = src->name;
		dest->discription = src->discription;
		dest->project_group_id = src->project_group_id;
	}

	void DataBaseTest::test_Projects_DataBase()
	{
		TIMEY_INFO("Create Project DataBase.");
		ProjectDb* projdb = new ProjectDb{ m_dbpath };

		std::vector<Ref<Project>> ramdom_projects;
		std::size_t projects_num = 100;

		//Generating data
		TIMEY_INFO("Generating {} ramdom projects.", projects_num);
		for (int i = 0; i < projects_num; i++) {
			ramdom_projects.push_back(genProjects());
		}

		// Inserting data into database
		TIMEY_INFO("Inserting ramdom projects into {}.", m_dbpath);
		for (auto p : ramdom_projects) {
			projdb->insertData(*p);
		};

		TIMEY_INFO("Fetching each project by ID and compare them to the expected value.");
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

		TIMEY_INFO("Generating new {} projects and updating the database.", projects_num);
		// Updating test.
		for (int i = 1; i <= projects_num; i++) {
			Ref<Project> p1 = projdb->fetchData(i);
			Ref<Project> p2 = genProjects();
			copyProjectContent(p2, p1);
			projdb->updateData(*p1);
			ramdom_projects.push_back(p1);

		};

		TIMEY_INFO("Comparing the update value with the database.");
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

		TIMEY_INFO("Deleteing all data");
		for (int i = 1; i <= projects_num; i++) {
			projdb->deleteData(i);
		};


		TIMEY_INFO("Quering the database to see if the database is empty.");
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

	};

};