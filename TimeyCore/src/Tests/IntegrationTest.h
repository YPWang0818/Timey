#include "timey_pch.h"


namespace Timey {

	class Project;

	class IntegrationTest {

	public:
		IntegrationTest(const std::string& name) 
			:m_testName{name}
		{};

		virtual void run() = 0;

		static int randNumber(int max, int min = 0);
		static float randReal(float max = 1.0, float min = 0.0);
		static std::string randStrings(std::size_t len);


	private:
		std::string m_testName;
	};

	class DataBaseTest : public IntegrationTest {

	public:
		DataBaseTest(const std::string& name, const std::string& dbpath)
			:IntegrationTest{ name }, m_dbpath{dbpath}
		{
			namespace fs = std::filesystem;
			if (fs::is_regular_file(dbpath)) {
				TIMEY_CORE_WARN("Old database file {} detected and removed.", dbpath);
				fs::remove(dbpath);
			};

		};

		virtual void run() override;

	private:
		Ref<Project> genProjects();
		void test_Projects_DataBase();

	private:
		std::string m_dbpath; // = "../databases/test.db";

	};


}