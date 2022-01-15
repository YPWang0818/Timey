#include "timey_pch.h"
#include "Core\DbClasses\DataBase.h"

namespace Timey {

	struct Project;
	struct Session;
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

			sessiondb = CreateScope<SessionDb>(m_dbpath);
			projdb = CreateScope<ProjectDb>(m_dbpath);
			tagdb = CreateScope<TagDb>(m_dbpath);
			projgpdb = CreateScope<ProjectGroupDb>(m_dbpath);
			taggpdb = CreateScope<TagGroupDb>(m_dbpath);
		};

		virtual void run() override;

	private:
		Ref<Project> genProjects();
		Ref<Session> genSessions();
		Ref<Tag> genTags();
		Ref<ProjectGroup> genProjectGroups();
		Ref<TagGroup> genTagGroups();

		void copyProjectContent(const Ref<Project> src, Ref<Project> dest); // Won't copy the id.
		void copySessionContent(const Ref<Session> src, Ref<Session> dest); 
		void copyTagContent(const Ref<Tag> src, Ref<Tag> dest);
		void copyProjectGroupContent(const Ref<ProjectGroup> src, Ref<ProjectGroup> dest);
		void copyTagGroupContent(const Ref<TagGroup> src, Ref<TagGroup> dest);

		void test_Projects_DataBase();
		void test_Sessions_DataBase();
		void test_Tags_DataBase();
		void test_ProjectGroup_DataBase();
		void test_TagGroup_DataBase();



	private:
		std::string m_dbpath; // = "../databases/test.db";
		Scope<SessionDb>  sessiondb;
		Scope<ProjectDb> projdb;
		Scope<TagDb> tagdb;
		Scope<ProjectGroupDb> projgpdb;
		Scope<TagGroupDb> taggpdb;

	};




}