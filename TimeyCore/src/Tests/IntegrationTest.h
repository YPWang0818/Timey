#include "timey_pch.h"


namespace Timey {

	struct Project;
	class IntegrationTestSuite;

	class IntegrationTests {

		IntegrationTests();
		~IntegrationTests();

		std::vector<Scope<IntegrationTestSuite>> m_integrationTest;
		static IntegrationTests* m_instance;

		inline auto& getTestsInternals() { return m_integrationTest; };
	public:
		static IntegrationTests& getTests() {
			if (!m_instance) {
				m_instance = new IntegrationTests;
			};
			return *m_instance;
		};

		static void Run();
	};

	class IntegrationTestSuite {

	public:
		IntegrationTestSuite(const std::string& name) 
			:m_testName{name}
		{};

		virtual void run() = 0;
		virtual void setup();
		virtual void teardown();

		static int randNumber(int max, int min = 0);
		static float randReal(float max = 1.0, float min = 0.0);
		static std::string randString(std::size_t len);


	private:
		std::string m_testName;
	};

	class DataBaseTest : public IntegrationTestSuite {

	public:
		DataBaseTest(const std::string& name, const std::string& dbpath)
			:IntegrationTestSuite{ name }, m_dbpath{dbpath}
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
		void copyProjectContent(const Ref<Project> src, Ref<Project> dest); // Won't copy the id.

		void test_Projects_DataBase();

	private:
		std::string m_dbpath; // = "../databases/test.db";

	};


}