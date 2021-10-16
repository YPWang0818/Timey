#include "timey_pch.h"
#include <filesystem>
#include "DataBase.h"


namespace Timey {

	namespace TestUtil {

		static int rand_num(int max, int min = 0 ) {
		
			if (max <= min) return min;

			static std::random_device randDev;
			static std::mt19937 Gen(randDev());
			static std::uniform_int_distribution<std::size_t> dist;

			return min + ( dist(Gen) % (max - min + 1) );
		}

		static double rand_real(double max = 1.0, double min = 0.0) {
			if (max <= min) return min;
			static std::random_device randDev;
			static std::mt19937 Gen(randDev());

			std::uniform_real_distribution dist(min, max);
			return dist(Gen);

		}





	};

	static struct SessionID

	{

		SessionID(int duration, uint64_t start, uint64_t end, char* title, const std::string& discription)
			:duration{ duration }, startTime{ start }, endTime{ end }, title{ title }, description{discription}
		{};


		int duration;
		uint64_t startTime;
		uint64_t endTime;

		char* title;
		std::string description;

		using s_duration_ = typename ComponetInfo<"session_id", decltype(duration)>;
		using s_startTime_ = typename ComponetInfo<"startTime", decltype(startTime), NotNull>;
		using s_endtime_ = typename ComponetInfo<"endTime", decltype(endTime), NotNull>;
		using s_title_ = typename ComponetInfo<"title", decltype(title), NotNull, Unique>;
		using s_description_ = typename ComponetInfo<"description", decltype(description)>;

		Components<s_duration_, s_startTime_, s_endtime_, s_title_, s_description_> comp_{ &duration, &startTime, &endTime, &title, &description };
		using comps_tuple_ = typename Components<s_duration_, s_startTime_, s_endtime_, s_title_, s_description_>::compsTuple_t;
	};



	class SessionDb : public DataBase<"SessionDb", SessionID>
	{
	public:
		SessionDb(const std::string& filepath)
			: DataBase<"SessionDb", SessionID>{ filepath }
		{};


	};



	static struct ProjectID {

		ProjectID(float r, float g, float b, float a, char* title, const std::string& description) 
			: colorR{ r }, colorG{ g }, colorB{ b }, colorA{ a }, title{ title }, description{description}
		{};


		float colorR;
		float colorG;
		float colorB;
		float colorA;

		char* title;
		std::string description;
	

		using  p_colorR_ = ComponetInfo<"colorR", decltype(colorR), NotNull,
			CheckExpr<"0 <= colorR AND 1 >= colorR">>;
		using  p_colorG_ = ComponetInfo<"colorG", decltype(colorG), NotNull,
			CheckExpr<"0 <= colorG AND 1 >= colorG">>;
		using  p_colorB_ = ComponetInfo<"colorB", decltype(colorB), NotNull,
			CheckExpr<"0 <= colorB AND 1 >= colorB">>;
		using  p_colorA_ = ComponetInfo<"colorA", decltype(colorA), NotNull,
			CheckExpr<"0 <= colorA AND 1 >= colorA">>;

		using p_title_ = ComponetInfo<"title", decltype(title), NotNull, Unique>;
		using p_description_ = ComponetInfo<"description", decltype(description)>;

		Components< p_colorR_, p_colorG_, p_colorB_, p_colorA_, p_title_, p_description_ > comp_{ &colorR, &colorG, &colorB, &colorA, &title, &description };
		using comps_tuple_ = Components< p_colorR_, p_colorG_, p_colorB_, p_colorA_, p_description_, p_title_>::compsTuple_t;

	};


	class ProjectDb : public DataBase<"ProjectDb", ProjectID>
	{
	public:
		ProjectDb(const std::string& filepath)
			: DataBase<"ProjectDb", ProjectID>{ filepath }
		{};


	};



	static void db_test() {

		const std::string dbpath = "../databases/test.db";

		SessionDb db(dbpath);
		ProjectDb pdb(dbpath);

		const int sNum = 10;
		SessionID* sessions[sNum];
		for (int i = 0; i < sNum; i++) {
			sessions[i] = new SessionID(
				TestUtil::rand_num(1024),
				TestUtil::rand_num(100000),
				TestUtil::rand_num(100000),
				"Some Title",
				"Some Description."
			);
			db.InsertRow(*sessions[i]);
		};

		const int pNum = 10;
		ProjectID* projects[pNum];
		for (int i = 0; i < pNum; i++) {
			projects[i] = new ProjectID(
				TestUtil::rand_real(1.0f),
				TestUtil::rand_real(1.0f),
				TestUtil::rand_real(1.0f),
				TestUtil::rand_real(1.0f),
				"Some Title",
				"Some Description."
			);
			pdb.InsertRow(*projects[i]);
		};



	
		TIMEY_CORE_TRACE((const char*)SessionDb::createTableStmt);
		TIMEY_CORE_TRACE((const char*)ProjectDb::createTableStmt);


		TIMEY_CORE_TRACE((const char*)SessionDb::insertStmt);
		TIMEY_CORE_TRACE((const char*)ProjectDb::insertStmt);


		TIMEY_CORE_TRACE((const char*)SessionDb::selectStmt);
		TIMEY_CORE_TRACE((const char*)ProjectDb::selectStmt);


		TIMEY_CORE_TRACE((const char*)SessionDb::deleteStmt);
		TIMEY_CORE_TRACE((const char*)ProjectDb::deleteStmt);


		TIMEY_CORE_TRACE((const char*)SessionDb::updateStmt);
		TIMEY_CORE_TRACE((const char*)ProjectDb::updateStmt);

	};

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

		TIMEY_TRACE(ss.str());

	};

	void print_res() {

		//sqlite_db_test();
		db_test();


	};
 }