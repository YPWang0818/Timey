#pragma once
#include"timey_pch.h"
#include "Core/DbClasses/CoreDataBase.h"
#include "Core/Object.h"

namespace Timey {

	class SessionDataBase : public CoreDataBase
	{

	public:
		SessionDataBase(const std::string& filename);
		void AddSession(const Session& session);
		void DeleteSession(const Session& session);
		void FetchSession(uint32_t session_id);

	private:
		void _setup_add_session_query();

	};


}