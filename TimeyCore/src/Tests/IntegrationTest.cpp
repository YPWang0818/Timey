#include "timey_pch.h"
#include "IntegrationTest.h"
#include "DbClasses/DataBase.h"


namespace Timey {



	int IntegrationTest::randNumber(int max, int min)
	{
		if (max <= min) return min;

		static std::random_device randDev;
		static std::mt19937 Gen(randDev());
		static std::uniform_int_distribution<std::size_t> dist;

		return min + (dist(Gen) % (max - min + 1));
	};

	float IntegrationTest::randReal(float max, float min)
	{
		if (max <= min) return min;
		static std::random_device randDev;
		static std::mt19937 Gen(randDev());

		std::uniform_real_distribution dist(min, max);
		return dist(Gen);
	};

	std::string IntegrationTest::randStrings(std::size_t len)
	{
		return std::string();
	};

	void DataBaseTest::run() {
		test_Projects_DataBase();
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
		ProjectDb* projdb = new ProjectDb{ m_dbpath };

		std::vector<Ref<Project>> ramdom_projects;
		std::size_t projects_num = 100;
		for (int i = 0; i < projects_num; i++) {
			ramdom_projects.push_back(genProjects());
		}

		// Insert Ramdom data.
		for (auto p : ramdom_projects) {
			projdb->insertData(*p);
		};

		// Fetch all of them
		for (int i = 1; i <= projects_num; i++) {
			Ref<Project> p1 = projdb->fetchData(i);
			Ref<Project> p2 = ramdom_projects[i - 1];

		};





	};

};