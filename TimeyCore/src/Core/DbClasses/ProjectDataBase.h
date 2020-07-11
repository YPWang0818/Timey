#pragma once
#include "Core/DbClasses/CoreDataBase.h"
#include "Core/Object.h"

namespace Timey {

	class ProjectDataBase : public CoreDataBase
	{
	public:
		ProjectDataBase(const std::string& filename);

		void AddProject(const Project& project);
		void DeleteProject(const Project& project);
		std::shared_ptr<Project> FetchProject(uint32_t project_id);

	private:
		void _setup_add_project_query();
		void _setup_fetch_project_query();
		void _setup_delete_project_query();
	};




	};












