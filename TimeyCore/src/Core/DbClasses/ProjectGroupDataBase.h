#pragma once
#include "Core/DbClasses/GroupDataBase.h"


namespace Timey {

	class ProjectGroupDataBase : public GroupDataBase {

	public:
		ProjectGroupDataBase(const std::string& filename);

		inline void AddProjectGroup(const ProjectGroup& project_group) { _add_group_object(project_group); };
		inline void DeleteProjectGroup(const ProjectGroup& project_group) { _delete_group_object(project_group); };

		inline std::shared_ptr<ProjectGroup> FetchProjectGroup(uint32_t project_group_id) {
			return std::static_pointer_cast<ProjectGroup>(_fetch_group_object(project_group_id));
		};


	private:
		virtual void _setup_add_group_object_query() override;
		virtual void _setup_delete_group_object_query() override;
		virtual void _setup_fetch_group_object_query() override;
		virtual void _setup_get_children_group_ID_query() override;

		virtual void _add_group_object(const GroupObject& group_obj) override;
		virtual void _delete_group_object(const GroupObject& group_obj) override;
		virtual std::shared_ptr<GroupObject> _fetch_group_object(uint32_t group_obj_id) override;
		virtual std::vector<int> _get_children_group_ID() override;


	};


}