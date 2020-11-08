#include"timey_pch.h"
#include "DbClasses/SessionDataBase.h"

namespace Timey {

	SessionDataBase::SessionDataBase(const std::string& filename)
	:CoreDataBase(filename)
	{
		_setup_add_session_query();
		_setup_fetch_session_query();
		_setup_delete_session_query();
	}


	void SessionDataBase::AddSession(const Session& session)
	{
		// Needs to deal with the case when there is already a session_id.

		std::shared_ptr<Query> add_session = getQueries()["add_session"];

		add_session->Bind<double>(1, static_cast<double>(session.duration));
		add_session->Bind<int>(2, session.start_time.date.year);
		add_session->Bind<int>(3, session.start_time.date.month);
		add_session->Bind<int>(4, session.start_time.date.day);
		add_session->Bind<int>(5, session.start_time.time.hour);
		add_session->Bind<int>(6, session.start_time.time.minute);
		add_session->Bind<int>(7, session.start_time.time.second);
		add_session->Bind<int>(8, session.end_time.date.year);
		add_session->Bind<int>(9, session.end_time.date.month);
		add_session->Bind<int>(10, session.end_time.date.day);
		add_session->Bind<int>(11, session.end_time.time.hour);
		add_session->Bind<int>(12, session.end_time.time.minute);
		add_session->Bind<int>(13, session.end_time.time.second);
		add_session->Bind<const std::string& >(14, session.name);
		add_session->Bind<const std::string& >(15, session.discription);
		add_session->Bind<int>(16, session.project_id);
	

		add_session->ExecOnceNoRes();
		add_session->Unbind();

	}
	void SessionDataBase::DeleteSession(const Session& session)
	{
		std::shared_ptr<Query> delete_session = getQueries()["delete_session"];

		if (!session.ID) {
			TIMEY_CORE_WARN("No session ID found.");
			return;
		}
		this->FetchSession(session.ID);// Write error msg when no session is found. 

		delete_session->Bind<int>(1, session.ID);
		delete_session->ExecOnceNoRes();
		delete_session->Unbind();
	}

	std::shared_ptr<Session> SessionDataBase::FetchSession(uint32_t session_id)
	{
		std::shared_ptr<Query> fetch_session = getQueries()["fetch_session"];
		fetch_session->Bind<int>(1, session_id);

		auto res = std::unique_ptr<QueryResult>(fetch_session->Exec());

		if (res->getRowCount() == 0) {
			fetch_session->Unbind();
			TIMEY_CORE_ERROR("No session found. Return Nullptr.");
			return nullptr; 
		};

		DateTime start_time = { 
			(uint32_t)( (res->getColumn<int>(2))[0]), //y
			(uint32_t)( (res->getColumn<int>(3))[0]), //m
			(uint32_t)( (res->getColumn<int>(4))[0]), //d
			(uint32_t)( (res->getColumn<int>(5))[0]), //h
			(uint32_t)( (res->getColumn<int>(6))[0]), //m
			(uint32_t)( (res->getColumn<int>(7))[0]), //s
		};

		DateTime end_time = {
			(uint32_t)((res->getColumn<int>(8))[0]), //y
			(uint32_t)((res->getColumn<int>(9))[0]), //m
			(uint32_t)((res->getColumn<int>(10))[0]), //d
			(uint32_t)((res->getColumn<int>(11))[0]), //h
			(uint32_t)((res->getColumn<int>(12))[0]), //m
			(uint32_t)((res->getColumn<int>(13))[0]), //s
		};
		


		std::string discription;

		if (res->getColumnType(15) != SQLiteType::null) {
			discription = (res->getColumn<std::string>(15))[0];
		}
		else {
			discription = "";
		};


		Session* session = new Session (
			(res->getColumn<int>(0))[0], // session_id
			(res->getColumn<std::string>(14))[0], // title
			discription, // discription
			start_time, // start time
			end_time, // end time
			(res->getColumn<double>(1))[0], //duration
			(res->getColumn<int>(16))[0] // project id
		);

		fetch_session->Unbind();
		return std::shared_ptr<Session>(session);
	};

	void SessionDataBase::_setup_add_session_query()
	{
		static const std::string add_session_query = R"(INSERT INTO sessions (duration, start_year , start_month, start_day, start_hour, start_minute, start_second, end_year, end_month, end_day, end_hour, end_minute, end_second, title, discription, project_id)      VALUES(?1, ?2 ,?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, ?13, ?14, ?15, ?16);
			)";


		this->AddQuery("add_session", std::make_shared<Query>(add_session_query, this));

	}

	void SessionDataBase::_setup_fetch_session_query()
	{
		static const std::string fetch_session_query = R"(
		SELECT * FROM sessions WHERE session_id == ?1;
		)";

		this->AddQuery("fetch_session", std::make_shared<Query>(fetch_session_query, this));

	}
	void SessionDataBase::_setup_delete_session_query()
	{
		static const std::string delete_session_query = R"(
		DELETE FROM sessions WHERE session_id == ?1 ;
		)";

		this->AddQuery("delete_session", std::make_shared<Query>(delete_session_query, this));

	}
}