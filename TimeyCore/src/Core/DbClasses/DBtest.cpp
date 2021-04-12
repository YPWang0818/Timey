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

		Components<s_duration_, s_startTime_, s_endtime_, s_title_, s_description_> comp_{&duration, &startTime, &endTime, &title, &description};
		using comps_tuple_ = typename Components<s_duration_, s_startTime_, s_endtime_, s_title_, s_description_>::compsTuple_t;
	};



	class SessionDb : public DataBase<"SessionDb", SessionID>
	{
	public:
		SessionDb(const std::string& filepath)
			: DataBase<"SessionDb", SessionID>{filepath}
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

		using p_title_= ComponetInfo<"title", decltype(title), NotNull, Unique>;
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

	void print_res() {

		/*
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
		*/

		SQLiteEntry e1;
		SQLiteEntry e2 = 12;
		SQLiteEntry e3 = 4.4f;

		SQLiteEntry e4 = { (void*)"Hello World", sizeof("Hello World") };


		TIMEY_CORE_TRACE(e4.data.c);

	};
 }