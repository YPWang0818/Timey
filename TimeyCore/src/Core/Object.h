#pragma once
#include "timey_pch.h"

namespace Timey {

	// The abstract class for sessions, tags, projects, tags group, project group.
	struct BaseObject {
		std::string name;
		uint32_t ID = 0; // ID = 0 implies the ID is not retrieved form the database yet.
	protected:
		BaseObject(const std::string name)
			:name(name){};
	};

	struct Color {
		float r;
		float g;
		float b;
		float a;
	};

	struct Date {
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
	};

	struct Tag : public BaseObject {
		Color tag_color;
		int tag_group_id;
		Tag(const std::string& name, Color color, int taggp_id)
			:BaseObject(name), tag_color(color), tag_group_id(taggp_id) {};
	};

	struct Session : public BaseObject
	{
		std::string discription;
		float duration;
		Date start_time;
		Date end_time;
		std::vector<std::shared_ptr<Tag>> tag_list;

		Session(const std::string& name, const std::string discription, Date start_time, Date end_time, float duration)
			:BaseObject(name), discription(discription), start_time(start_time), end_time(end_time), duration(duration) {
			tag_list = std::vector<std::shared_ptr<Tag>>();
		};
	};

	struct Project : public BaseObject
	{
		std::string discription;
		Color project_color;
		int project_group_id;
		std::vector<std::shared_ptr<Tag>> tag_list;

		Project(const std::string& name, const std::string& discription, Color color, int taggp_id)
			:BaseObject(name), project_color(color), discription(discription), project_group_id(taggp_id) {
			tag_list = std::vector<std::shared_ptr<Tag>>();
		};
	};

}