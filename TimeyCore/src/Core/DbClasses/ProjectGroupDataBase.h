#pragma once
#include "DbClasses/GroupDataBase.h"


namespace Timey {

	class ProjectGroupDataBase : public GroupDataBase {

	public:
		ProjectGroupDataBase(const std::string& filename);
	private:
		virtual void _setup_add_group_object_query() override;
		virtual void _setup_delete_group_object_query() override;
		virtual void _setup_fetch_group_object_query() override;
		virtual void _setup_get_children_group_ID_query() override;

		virtual void _add_group_object(const GroupObject& group_obj) override;
		virtual void _delete_group_object(const GroupObject& project_gp) override;
		virtual std::shared_ptr<GroupObject> _fetch_group_object(uint32_t project_gp_id) override;
		virtual std::vector<int> _get_children_group_ID() override;


	};


}