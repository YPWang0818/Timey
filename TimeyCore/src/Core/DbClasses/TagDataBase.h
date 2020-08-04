#pragma once

#include "Core/Object.h"
#include "Core/DbClasses/CoreDataBase.h"


namespace Timey {

	class TagDataBase : public CoreDataBase
	{
	public:
		TagDataBase(const std::string& filename);
		void AddTag(const Tag& tag);
		void DeleteTag(const Tag& tag);
		std::shared_ptr<Tag> FetchTag(uint32_t tag_id);

		void UpdateTagsGroup(const Tag& tag, const TagGroup& group);

	private:
		void _setup_add_tag_query();
		void _setup_fetch_tag_query();
		void _setup_delete_tag_query();
		void _setup_update_tags_group_query();


	};






}