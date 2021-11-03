#include "timey_pch.h"
#include <filesystem>
#include "DataBase.h"


namespace Timey {

	static const std::string sqlite_db_test_create_table_str = R"(
	-- create session table
	CREATE TABLE sessions (
	session_id		INTEGER		 PRIMARY KEY AUTOINCREMENT,
	duration		REAL 	NOT NULL,

	start_year		INTEGER NOT NULL, 
	start_month		INTEGER NOT NULL CHECK( 1 <= start_month AND 12 >= start_month),
	start_day		INTEGER NOT NULL CHECK( 1 <= start_day AND 31 >= start_day),
	start_hour		INTEGER NOT NULL CHECK( 0 <= start_hour AND 24 >= start_hour),
	start_minute	INTEGER NOT NULL CHECK( 0 <= start_minute AND 60 > start_minute),
	start_second	INTEGER NOT NULL CHECK( 0 <= start_second AND 60 > start_second),

	end_year		INTEGER NOT NULL, 
	end_month		INTEGER NOT NULL CHECK( 1 <= end_month AND 12 >= end_month),
	end_day			INTEGER NOT NULL CHECK( 1 <= end_day AND 31 >= end_day),
	end_hour		INTEGER NOT NULL CHECK( 0 <= end_hour AND 24 >= end_hour),
	end_minute		INTEGER NOT NULL CHECK( 0 <= end_minute AND 60 > end_minute),
	end_second		INTEGER NOT NULL CHECK( 0 <= end_second AND 60 > end_second),

	title			TEXT NOT NULL,
	discription		TEXT,
	project_id		INTEGER,

	FOREIGN KEY (project_id)  REFERENCES projects (project_id) 
    	ON DELETE RESTRICT
    	ON UPDATE CASCADE);
	)";

	static const std::string sqlite_db_test_insert_str = R"(INSERT INTO sessions (duration, start_year , start_month, start_day, 
	start_hour, start_minute, start_second, end_year, end_month, end_day, end_hour, end_minute, end_second,
	title, discription, project_id)      VALUES(?1, ?2 ,?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, ?13, ?14, ?15, ?16);
	)";

	static const std::string sqlite_db_test_select_str = R"( SELECT * FROM sessions WHERE session_id == ?1; )";
	static const std::string sqlite_db_test_selectAll_str = "SELECT * FROM sessions;";

	namespace TestUtil {

		static int rand_num(int max, int min = 0 ) {
		
			if (max <= min) return min;

			static std::random_device randDev;
			static std::mt19937 Gen(randDev());
			static std::uniform_int_distribution<std::size_t> dist;

			return min + ( dist(Gen) % (max - min + 1) );
		}

		static float rand_real(float max = 1.0, float min = 0.0) {
			if (max <= min) return min;
			static std::random_device randDev;
			static std::mt19937 Gen(randDev());

			std::uniform_real_distribution dist(min, max);
			return dist(Gen);

		}

		static Ref<Project> gen_projects() {

			Project* nwProject = new Project;

			nwProject->color = { rand_real(), rand_real(), rand_real(), rand_real() };
			nwProject->name = "Test Project Title.";
			nwProject->discription = "Some descrption";
			nwProject->project_group_id = rand_num(1024);

			Ref<Project> proj = CreateRef<Project>();
			proj.reset(nwProject);
			return proj;
		};



	};

	static void sqlite_db_test() {

		const std::string dbpath = "../databases/test.db";
		namespace fs = std::filesystem;
		if (fs::is_regular_file(dbpath)) {
			fs::remove(dbpath);
		}

		SqliteDb* db = new SqliteDb{ dbpath };
		SqliteQuery* q_create = new SqliteQuery(sqlite_db_test_create_table_str);
		SqliteQuery* q_insert = new SqliteQuery(sqlite_db_test_insert_str);
		SqliteQuery* q_select = new SqliteQuery(sqlite_db_test_select_str);
		SqliteQuery* q_selectAll = new SqliteQuery(sqlite_db_test_selectAll_str);

		q_create->compile(*db);
		q_create->exec();

		q_insert->compile(*db);

		std::size_t entry_count = 100;
		for (int i = 0; i < entry_count; i++) {

			q_insert->bindColumnReal(1, TestUtil::rand_real(1000.0));

			for (int c = 0; c < 2; c++) {
				q_insert->bindColumnInteger(c * 6 + 2, TestUtil::rand_num(2021));
				q_insert->bindColumnInteger(c * 6 + 3, TestUtil::rand_num(12, 1));
				q_insert->bindColumnInteger(c * 6 + 4, TestUtil::rand_num(31, 1));
				q_insert->bindColumnInteger(c * 6 + 5, TestUtil::rand_num(23));
				q_insert->bindColumnInteger(c * 6 + 6, TestUtil::rand_num(59));
				q_insert->bindColumnInteger(c * 6 + 7, TestUtil::rand_num(59));
			}

			q_insert->bindColumnText(14, "Some title.");
			q_insert->bindColumnText(15, "Description! Description! Description!");
			q_insert->bindColumnInteger(16, TestUtil::rand_num(1024));

			q_insert->exec();
			q_insert->unbindAllColumns();

		}

		q_select->compile(*db);
		q_select->bindColumnInteger(1, TestUtil::rand_num(entry_count, 1));

		q_selectAll->compile(*db);

		Ref<SqliteTable> result;
		if ((result = q_selectAll->exec()) == nullptr) { TIMEY_CORE_ERROR("Error in execution or empty table."); };
		
		std::stringstream ss;
		for (int i = 0; i < result->getColumnCount(); i++) {

			Ref<SqliteColumn> col = (*result)[i];
			for (auto& e : *col) {

				switch (e.type) {
				case SQLType::integer:
					ss << e.data.i << " \n ";
					break;
				case SQLType::real:
					ss << e.data.d << " \n ";
					break;
				case SQLType::null:
					ss << "NULL" << " \n ";
					break;
				case SQLType::text:
					ss << e.data.c << " \n ";
					break;
				case SQLType::blob:
					ss << e.data.v << " \n ";
					break;
				default:
					break;
				}
			}
		};

		TIMEY_TRACE(result->toString());

	};

	static void project_db_test() {


		std::string projDBPath = "../databases/testProj.db";
		namespace fs = std::filesystem;
		if (fs::is_regular_file(projDBPath)) {
			fs::remove(projDBPath);
		};

		ProjectDb* projdb = new ProjectDb{projDBPath};
		
		std::vector<Ref<Project>> ramdom_projects;
		std::size_t projects_num = 100;
		for (int i = 0; i < projects_num; i++) {
			ramdom_projects.push_back(TestUtil::gen_projects());
		}

		// Insert Ramdom data.
		for (auto p : ramdom_projects) {
			projdb->insertData(*p);
		};

		// Fetch all of them
		for (int i = 1; i <= projects_num; i++) {
			Ref<Project> p1 = projdb->fetchData(i);
			Ref<Project> p2 = ramdom_projects[i - 1];

		}

	};

	void print_res() {
		project_db_test();
	};
 }