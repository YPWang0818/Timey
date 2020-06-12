#include"timey_pch.h"
#include "DbClasses/SessionDataBase.h"

namespace Timey {

	SessionDataBase::SessionDataBase(const std::string& filename)
	:CoreDataBase(filename)
	{
		_setup_add_session_query();
	}


	void SessionDataBase::AddSession(const Session& session)
	{
		std::shared_ptr<Query> add_session = getQueries()["add_session"];

		add_session->Bind<double>(1, static_cast<double>(session.duration));
		add_session->Bind<int>(2, session.start_time.year);
		add_session->Bind<int>(3, session.start_time.month);
		add_session->Bind<int>(4, session.start_time.day);
		add_session->Bind<int>(5, session.start_time.hour);
		add_session->Bind<int>(6, session.start_time.minute);
		add_session->Bind<int>(7, session.start_time.second);
		add_session->Bind<int>(8, session.end_time.year);
		add_session->Bind<int>(9, session.end_time.month);
		add_session->Bind<int>(10, session.end_time.day);
		add_session->Bind<int>(11, session.end_time.hour);
		add_session->Bind<int>(12, session.end_time.minute);
		add_session->Bind<int>(13, session.end_time.second);
		add_session->Bind<const std::string& >(14, session.name);
		add_session->Bind<const std::string& >(15, session.discription);

		add_session->ExecOnceNoRes();
		add_session->Unbind();

	}
	void SessionDataBase::DeleteSession(const Session& session)
	{
	}
	void SessionDataBase::FetchSession(uint32_t session_id)
	{
	}
	void SessionDataBase::_setup_add_session_query()
	{
		static const std::string add_session_query = R"(INSERT INTO sessions (duration, start_year , start_month, start_day, start_hour, start_minute, start_second, end_year, end_month, end_day, end_hour, end_minute, end_second, title, discription)      VALUES(?1, ?2 ,?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, ?13, ?14, ?15);
			)";

		std::shared_ptr<Query> add_session = std::make_shared<Query>(add_session_query, this);

		this->AddQuery("add_session", add_session);

	}
}