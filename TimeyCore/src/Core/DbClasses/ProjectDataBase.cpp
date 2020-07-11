#include "timey_pch.h"
#include "Core/DbClasses/ProjectDataBase.h"


namespace Timey{


	ProjectDataBase::ProjectDataBase(const std::string& filename)
		:CoreDataBase(filename)
	{
		_setup_add_project_query();
		_setup_fetch_project_query();
		_setup_delete_project_query();

	};

	void ProjectDataBase::AddProject(const Project& project)
	{
		std::shared_ptr<Query> add_project = getQueries()["add_project"];

		add_project->Bind<double>(1, static_cast<double>(project.project_color.r));
		add_project->Bind<double>(2, static_cast<double>(project.project_color.g));
		add_project->Bind<double>(3, static_cast<double>(project.project_color.b));
		add_project->Bind<double>(4, static_cast<double>(project.project_color.a));

		add_project->Bind<const std::string&>(5, project.name );
		add_project->Bind<const std::string&>(6, project.discription);
		add_project->Bind<int>(7, project.project_group_id);

		add_project->ExecOnceNoRes();
		add_project->Unbind();

	}

	void ProjectDataBase::DeleteProject(const Project& project)
	{
		std::shared_ptr<Query> delete_project = getQueries()["delete_project"];

		if (!project.ID) {
			TIMEY_CORE_WARN("No project ID found.");
			return;
		}

		delete_project->Bind<int>(1, project.ID);
		delete_project->ExecOnceNoRes();
		delete_project->Unbind();
	}

	std::shared_ptr<Project> ProjectDataBase::FetchProject(uint32_t project_id)
	{
		std::shared_ptr<Query> fetch_project = getQueries()["fetch_project"];

		fetch_project->Bind<int>(1, project_id);
		auto res = std::unique_ptr<QueryResult>(fetch_project->Exec());

		if (res->getRowCount() == 0) {
			fetch_project->Unbind();
			TIMEY_CORE_ERROR("No project found. Return Nullptr.");
			return nullptr;
		};


		Color color = {
			(res->getColumn<double>(1))[0], // r
			(res->getColumn<double>(2))[0], // g
			(res->getColumn<double>(3))[0], // b
			(res->getColumn<double>(4))[0]  // a
		};


		std::string discription;

		if (res->getColumnType(6) != SQLiteType::null) {
			discription = (res->getColumn<std::string>(6))[0];
		}
		else {
			discription = "";
		};


		Project* project = new Project(
			(res->getColumn<int>(0))[0], // project_id
			(res->getColumn<std::string>(5))[0], // name
			discription,
			color,
			(res->getColumn<int>(7))[0] // project group id
		);

		return std::shared_ptr<Project>(project);
	}

	void ProjectDataBase::_setup_add_project_query()
	{
		static const std::string add_project_query = R"(INSERT INTO projects (color_r, color_g, color_b, color_a, title, discription, projects_group_id) VALUES(?1, ?2, ?3, ?4, ?5, ?6, ?7);
		)";

		this->AddQuery("add_project", std::make_shared<Query>(add_project_query, this));
	}

	void ProjectDataBase::_setup_fetch_project_query()
	{

		static const std::string fetch_project_query = R"(
		SELECT * FROM projects WHERE project_id == ?1;
		)";

		this->AddQuery("fetch_project", std::make_shared<Query>(fetch_project_query, this));
	}

	void ProjectDataBase::_setup_delete_project_query()
	{

		static const std::string delete_project_query = R"(
		DELETE FROM projects WHERE project_id == ?1 ;
		)";

		this->AddQuery("delete_project", std::make_shared<Query>(delete_project_query, this));
	}

}