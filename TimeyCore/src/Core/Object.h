#pragma once
#include "timey_pch.h"

namespace Timey {

	// The abstract class for sessions, tags, projects, tags group, project group.
	struct BaseObject {
		std::string name;
		uint32_t ID; 

		virtual std::string toString() const = 0;
	protected:
		BaseObject(const std::string name, uint32_t id)
			:name(name), ID(id) {};

		BaseObject(const std::string name)
			:BaseObject(name, 0) // ID = 0 implies the ID haven't been retrieved form the database yet.
		{};

		
	};

	struct Color {
		float r;
		float g;
		float b;
		float a;

		std::string toString() const {
			std::stringstream ss;
			ss << "(" << r << "," << g << "," << b << "," << a << ")";
			return ss.str();
		}
	};

	struct Date {
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;

		std::string toString() const {
			std::stringstream ss;
			ss << year << ":" << month << ":" << day << " " << hour << ":" << minute << ":" << second;
			return ss.str();
		}
		
	};

	struct Tag : public BaseObject {
		Color tag_color;
		int tag_group_id;

		Tag(uint32_t ID, const std::string& name, Color color, int taggp_id)
			:BaseObject(name, ID), tag_color(color), tag_group_id(taggp_id) {};

		Tag( const std::string& name, Color color, int taggp_id)
			:Tag(0, name, color, taggp_id) {};

		virtual std::string toString() const  override {

			std::stringstream ss;
			ss << "Name: " << name << "\n";
			ss << "ID: " << ID << "\n";
			ss << "Color :" << tag_color.toString() << "\n";
			ss << "Tag Group ID: " << tag_group_id << "\n";

			return ss.str();
		}
	};

	struct Session : public BaseObject
	{
		std::string discription;
		float duration;
		Date start_time;
		Date end_time;
		uint32_t project_id;
		std::vector<std::shared_ptr<Tag>> tag_list;

		Session(uint32_t ID, const std::string& name, const std::string& discription, Date start_time, Date end_time, float duration, uint32_t project_ID)
			:BaseObject(name, ID),
			discription(discription),
			start_time(start_time),
			end_time(end_time), 
			duration(duration), 
			project_id(project_ID),
			tag_list(std::vector<std::shared_ptr<Tag>>())
		{};

		Session(const std::string& name, const std::string discription, Date start_time, Date end_time, float duration, uint32_t project_ID = 0) 
			:Session(0, name, discription, start_time, end_time, duration, project_ID) 
		{}; // project_ID = 0 implies the session is in no projects.


		virtual std::string toString() const override {
			
			std::stringstream ss;

			ss << "Name: " << name << "\n";
			ss << "ID: " << ID << "\n";
			ss << "Duration: " << duration << "\n";
			ss << "Start time: " << start_time.toString() << "\n";
			ss << "End time: " << end_time.toString() << "\n";
			ss << "Discription: " << discription << "\n";
			ss << "Project ID:" << project_id << "\n";

			ss << "Tag Ids: \n";

			for (auto tag : tag_list) {
				ss << tag->ID << ", ";
 			}

			return ss.str();
		}



	};

	struct Project : public BaseObject
	{
		std::string discription;
		Color project_color;
		int project_group_id;
		std::vector<std::shared_ptr<Tag>> tag_list;

		Project(uint32_t ID, const std::string& name, const std::string& discription, Color color, int prjgp_id)
			:BaseObject(name, ID), project_color(color), discription(discription), project_group_id(prjgp_id), tag_list(std::vector<std::shared_ptr<Tag>>()) {};

		Project(const std::string& name, const std::string& discription, Color color, int prjgp_id)
			:Project(0, name, discription, color, prjgp_id)
		{}


		virtual std::string toString() const override {

			std::stringstream ss;
			ss << "Name: " << name << "\n";
			ss << "ID: " << ID << "\n";
			ss << "Color :" << project_color.toString() << "\n";
			ss << "Project Group ID: " << project_group_id << "\n";

			ss << "Tag Ids:\n";

			for (auto tag : tag_list) {
				ss << tag->ID << ", ";
			}
			return ss.str();
		}
	};


	inline std::ostream& operator<<(std::ostream& os, const BaseObject& obj) {
		return  os << obj.toString();
	};
}