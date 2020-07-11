#include "timey_pch.h"
#include "TagDataBase.h"


namespace Timey {


	TagDataBase::TagDataBase(const std::string& filename)
		:CoreDataBase(filename)
	{
		_setup_add_tag_query();
		_setup_fetch_tag_query();
		_setup_delete_tag_query();

	}

	void TagDataBase::AddTag(const Tag& tag)
	{
		std::shared_ptr<Query> add_tag = getQueries()["add_tag"];

		add_tag->Bind<double>(1, static_cast<double>(tag.tag_color.r));
		add_tag->Bind<double>(2, static_cast<double>(tag.tag_color.g));
		add_tag->Bind<double>(3, static_cast<double>(tag.tag_color.b));
		add_tag->Bind<double>(4, static_cast<double>(tag.tag_color.a));

		add_tag->Bind<const std::string&>(5, tag.name);
		add_tag->Bind<int>(6, tag.tag_group_id);

		add_tag->ExecOnceNoRes();
		add_tag->Unbind();
	}

	void TagDataBase::DeleteTag(const Tag& tag)
	{
		std::shared_ptr<Query> delete_tag = getQueries()["delete_tag"];

		if (!tag.ID) {
			TIMEY_CORE_WARN("No tag ID found.");
			return;
		}

		delete_tag->Bind<int>(1, tag.ID);
		delete_tag->ExecOnceNoRes();
		delete_tag->Unbind();
	}

	std::shared_ptr<Tag> TagDataBase::FetchTag(uint32_t tag_id)
	{
		std::shared_ptr<Query> fetch_tag = getQueries()["fetch_tag"];
		fetch_tag->Bind<int>(1, tag_id);

		auto res = std::unique_ptr<QueryResult>(fetch_tag->Exec());
		

		if (res->getRowCount() == 0) {
			fetch_tag->Unbind();
			TIMEY_CORE_ERROR("No tag found. Return Nullptr.");
			return nullptr;
		};

		Color color = {
			(res->getColumn<double>(1))[0], // r
			(res->getColumn<double>(2))[0], // g
			(res->getColumn<double>(3))[0], // b
			(res->getColumn<double>(4))[0]  // a
		};

		Tag* tag = new Tag(
			(res->getColumn<int>(0))[0],// tag id
			(res->getColumn<std::string>(5))[0], // name
			color,
			(res->getColumn<int>(6))[0] // tag group id
		);


		return std::shared_ptr<Tag>(tag);
	}

	void TagDataBase::_setup_add_tag_query()
	{
		static const std::string add_tag_query = R"( 
			INSERT INTO tags(color_r, color_g, color_b, color_a, title, tags_group_id)
			VALUES(?1, ?2, ?3, ?4, ?5, ?6);
		)";

		this->AddQuery("add_tag", std::make_shared<Query>(add_tag_query, this));

	}

	void TagDataBase::_setup_fetch_tag_query()
	{
		static const std::string fetch_tag_query = R"( 
			SELECT * FROM tags WHERE tag_id == ?1;
		)";

		this->AddQuery("fetch_tag", std::make_shared<Query>(fetch_tag_query, this));
	}

	void TagDataBase::_setup_delete_tag_query()
	{
		static const std::string delete_tag_query = R"(
		DELETE FROM tags WHERE tag_id == ?1 ;
		)";
	
		this->AddQuery("delete_tag", std::make_shared<Query>(delete_tag_query, this));

	}

}