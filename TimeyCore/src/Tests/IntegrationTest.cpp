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

	std::string IntegrationTestSuite::randStrings(std::size_t len)
	{
		return std::string();
	};

	void DataBaseTest::run() {
		test_Projects_DataBase();
	};

	void IntegrationTestSuite::setup() {
		TIMEY_TRACE("Start test suite {} ... \n", m_testName);
	};

	void IntegrationTestSuite::teardown() {
		TIMEY_TRACE(" End test suite {}", m_testName);
	};

	Ref<Project>  DataBaseTest::genProjects() {

		Project* nwProject = new Project;
		nwProject->color = { randReal(), randReal(), randReal(), randReal() };
		nwProject->name = "Test Project Title.";
		nwProject->discription = "Some descrption";
		nwProject->project_group_id = randNumber(1024);

		Ref<Project> proj = CreateRef<Project>();
		proj.reset(nwProject);
		return proj;
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

	

	};

};