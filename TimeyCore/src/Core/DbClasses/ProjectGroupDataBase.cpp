#include "timey_pch.h"
#include "DbClasses/ProjectGroupDataBase.h"

namespace Timey {



	ProjectGroupDataBase::ProjectGroupDataBase(const std::string& filename)
		:GroupDataBase(filename)
	{
		_setup_add_group_object_query();
		_setup_delete_group_object_query();
		_setup_fetch_group_object_query();
		_setup_get_children_group_ID_query();


	}

	void ProjectGroupDataBase::_setup_add_group_object_query()
	{
		// The implmentation for project group.

		const std::string add_projects_group_query = R"(
			INSERT INTO projects_group (name, parent_id) VALUES(1?, 2?);
		)";
		this->AddQuery("add_projects_group", std::make_shared<Query>(add_projects_group_query, this));
	}

	void ProjectGroupDataBase::_setup_delete_group_object_query()
	{
		static const std::string delete_projects_group_query = R"(
		DELETE FROM projects_group WHERE projects_group_id == ?1 ;
		)";
		this->AddQuery("delete_project_group", std::make_shared<Query>(delete_projects_group_query, this));
	}

	void ProjectGroupDataBase::_setup_fetch_group_object_query()
	{
		static const std::string fetch_projects_group_query = R"(
		SELECT * FROM projects_group WHERE projects_group_id == ?1;
		)";
		this->AddQuery("fetch_projects_group", std::make_shared<Query>(fetch_projects_group_query, this));
	}

	void ProjectGroupDataBase::_setup_get_children_group_ID_query()
	{
	}

	void ProjectGroupDataBase::_add_group_object(const GroupObject& group_obj)
	{
		std::shared_ptr<Query> add_projects_group = getQueries()["add_projects_group"];
		add_projects_group->Bind<const std::string&>(1, group_obj.name);
		add_projects_group->Bind<int>(1, group_obj.ID);

		add_projects_group->ExecOnceNoRes();
		add_projects_group->Unbind();
	}

	void ProjectGroupDataBase::_delete_group_object(const GroupObject& project_gp)
	{
		std::shared_ptr<Query> delete_projects_group = getQueries()["delete_projects_group"];

		if (!project_gp.ID) {
			TIMEY_CORE_WARN("No projects group ID found.");
			return;
		}
		this->_fetch_group_object(project_gp.ID) // Write error msg when no project_gp is found. 

		delete_projects_group->Bind<int>(1, project_gp.ID);
		delete_projects_group->ExecOnceNoRes();
		delete_projects_group->Unbind();
	}

	std::shared_ptr<GroupObject> ProjectGroupDataBase::_fetch_group_object(uint32_t project_gp_id)
	{

		std::shared_ptr<Query> fetch_projects_group = getQueries()["fetch_projects_group"];

		fetch_projects_group->Bind<int>(1, project_gp_id);
		auto res = std::unique_ptr<QueryResult>(fetch_projects_group->Exec());

		if (res->getRowCount() == 0) {
			fetch_projects_group->Unbind();
			TIMEY_CORE_ERROR("No projects group found. Return Nullptr.");
			return nullptr;
		};

		ProjectGroup* project_gp = new ProjectGroup(
			(res->getColumn<int>(0))[0], // project_id
			(res->getColumn<std::string>(1))[0], // name
			(res->getColumn<int>(2))[0] // parent_id
		);

		return std::shared_ptr<GroupObject>(project_gp);
	}

	std::vector<int> ProjectGroupDataBase::_get_children_group_ID()
	{
		return std::vector<int>();
	}

}