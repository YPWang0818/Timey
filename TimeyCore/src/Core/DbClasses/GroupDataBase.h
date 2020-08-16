#pragma once
#include "DbClasses/CoreDataBase.h"
#include "Core/Object.h"

namespace Timey {

	class GroupDataBase : public CoreDataBase
	{

	public:

		// Pure Virutal Class for Group Object related databases. The current databases are TagGroupDataBase and ProjectGroupDataBase
		GroupDataBase(const std::string& filename)
			:CoreDataBase(filename)
		{};

	private:

		virtual void _setup_add_group_object_query() = 0;
		virtual void _setup_delete_group_object_query() = 0;
		virtual void _setup_fetch_group_object_query() = 0;
		virtual void _setup_get_children_group_ID_query() = 0;

		virtual void _add_group_object(const GroupObject& group_obj) = 0;
		virtual void _delete_group_object(const GroupObject& project_gp) = 0;
		virtual std::shared_ptr<GroupObject> _fetch_group_object(uint32_t project_gp_id) = 0;
		virtual std::vector<int> _get_children_group_ID() = 0;

	};




}