#include "timey_pch.h"
#include "DataBase.h"


namespace Timey {



	static struct SessionID

	{

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

	static void init_database() {
		SessionDb db("../databases/test.db");
		ProjectDb pdb("../databases/test.db");
	};

	static void db_test() {

		init_database();
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

	static const std::string query_str = R"(
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

	static const std::string insert_str = R"(INSERT INTO sessions (duration, start_year , start_month, start_day, 
	start_hour, start_minute, start_second, end_year, end_month, end_day, end_hour, end_minute, end_second,
	title, discription, project_id)      VALUES(?1, ?2 ,?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, ?13, ?14, ?15, ?16);
	)";

	static void sqlite_db_test() {

		SqliteDb* db = new SqliteDb{ "../databases/test.db" };
		SqliteQuery* q = new SqliteQuery(query_str);
		SqliteQuery* q_insert = new SqliteQuery(insert_str);

		q->bindDb(*db);
		if (q->exec() == nullptr) { TIMEY_CORE_ERROR("Something is wrong.\n"); };
		
		q_insert->bindDb(*db);

		q_insert->bindColumnInteger(1, 1000);
		q_insert->bindColumnInteger(2, 1995);
		q_insert->bindColumnInteger(3, 8);
		q_insert->bindColumnInteger(4, 18);
		q_insert->bindColumnInteger(5, 9);
		q_insert->bindColumnInteger(6, 45);
		q_insert->bindColumnInteger(7, 36);
		q_insert->bindColumnInteger(8, 2021);
		q_insert->bindColumnInteger(9, 5);
		q_insert->bindColumnInteger(10, 17);
		q_insert->bindColumnInteger(11, 16);
		q_insert->bindColumnInteger(12, 27);
		q_insert->bindColumnInteger(13, 59);

		q_insert->bindColumnText(14, "Some title.");
		q_insert->bindColumnText(15, "Description! Description! Description!");
		q_insert->bindColumnInteger(16, 0);

		if (q_insert->exec() == nullptr) { TIMEY_CORE_ERROR("Something is wrong.\n"); };


	
	};


	void print_res() {

		sqlite_db_test();
		//db_test();


	};
 }