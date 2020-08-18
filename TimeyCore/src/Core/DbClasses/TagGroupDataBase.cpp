#include "timey_pch.h"
#include "Core/DbClasses/TagGroupDataBase.h"



namespace Timey {



	TagGroupDataBase::TagGroupDataBase(const std::string& filename)
		:GroupDataBase(filename)
	{
		_setup_add_group_object_query();
		_setup_delete_group_object_query();
		_setup_fetch_group_object_query();
		_setup_get_children_group_ID_query();
	}

	void TagGroupDataBase::_setup_add_group_object_query()
	{
		// The implmentation for tag group.

		const std::string add_tags_group_query = R"(
			INSERT INTO tags_group (name, parent_id) VALUES(?1, ?2);
		)";
		this->AddQuery("add_tags_group", std::make_shared<Query>(add_tags_group_query, this));

	}

	void TagGroupDataBase::_setup_delete_group_object_query()
	{
		static const std::string delete_tags_group_query = R"(
		DELETE FROM tags_group WHERE tags_group_id == ?1 ;
		)";
		this->AddQuery("delete_tags_group", std::make_shared<Query>(delete_tags_group_query, this));
	}

	void TagGroupDataBase::_setup_fetch_group_object_query()
	{
		static const std::string fetch_tags_group_query = R"(
		SELECT * FROM tags_group WHERE tags_group_id == ?1;
		)";
		this->AddQuery("fetch_tags_group", std::make_shared<Query>(fetch_tags_group_query, this));
	}

	void TagGroupDataBase::_setup_get_children_group_ID_query()
	{

	}

	void TagGroupDataBase::_add_group_object(const GroupObject& group_obj)
	{
		std::shared_ptr<Query> add_tags_group = getQueries()["add_tags_group"];
		add_tags_group->Bind<const std::string&>(1, group_obj.name);
		add_tags_group->Bind<int>(2, group_obj.parentID);

		add_tags_group->ExecOnceNoRes();
		add_tags_group->Unbind();

	}

	void TagGroupDataBase::_delete_group_object(const GroupObject& group_obj)
	{
		std::shared_ptr<Query> delete_tags_group = getQueries()["delete_tags_group"];

		if (!group_obj.ID) {
			TIMEY_CORE_WARN("No tags group ID found.");
			return;
		}
		this->_fetch_group_object(group_obj.ID); // Write error msg when no project_gp is found. 

		delete_tags_group->Bind<int>(1, group_obj.ID);
		delete_tags_group->ExecOnceNoRes();
		delete_tags_group->Unbind();
	}

	std::shared_ptr<GroupObject> TagGroupDataBase::_fetch_group_object(uint32_t group_obj_id)
	{
		std::shared_ptr<Query> fetch_tags_group = getQueries()["fetch_tags_group"];

		fetch_tags_group->Bind<int>(1, group_obj_id);
		auto res = std::unique_ptr<QueryResult>(fetch_tags_group->Exec());

		if (res->getRowCount() == 0) {
			fetch_tags_group->Unbind();
			TIMEY_CORE_ERROR("No tags group found. Return Nullptr.");
			return nullptr;
		};

		TagGroup* tag_gp = new TagGroup(
			(res->getColumn<int>(0))[0], // tag_id
			(res->getColumn<std::string>(1))[0], // name
			(res->getColumn<int>(2))[0] // parent_id
		);

		return std::shared_ptr<GroupObject>(tag_gp);
	}

	std::vector<int> TagGroupDataBase::_get_children_group_ID()
	{
		return std::vector<int>();
	}

}
