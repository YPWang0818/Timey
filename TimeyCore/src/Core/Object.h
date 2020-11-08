#pragma once
#include "timey_pch.h"
#include "TimeyTime.h"

namespace Timey {

	// The abstract class for sessions, tags, projects, tags group, project group.
	struct BaseObject {
		std::string name;
		uint32_t ID; 

		virtual std::string toString() const = 0;
	protected:
		BaseObject(uint32_t id, const std::string name)
			:ID(id), name(name) {};
		BaseObject(const std::string name)
			:name(name), ID(0) {}; // the initial ID is 0 if the object is created first time. 

		BaseObject() = default;
	};

	struct GroupObject : public BaseObject
	{
		uint32_t parentID;

	protected:
		GroupObject(uint32_t ID, const std::string& name, uint32_t parent_id)
			:BaseObject(ID, name), parentID(parent_id) {};
		GroupObject(const std::string& name, uint32_t parent_id)
			:BaseObject(name), parentID(parent_id) {};

		GroupObject() = default;
	};

	struct TagGroup : public GroupObject
	{
	public:
		TagGroup(uint32_t ID, const std::string& name, uint32_t parent_id)
			:GroupObject(ID, name, parent_id) {};
		TagGroup(const std::string& name, uint32_t parent_id)
			:GroupObject(name, parent_id) {};

		TagGroup() = default;

		std::string toString() const override {
			std::stringstream ss;
			ss << "Name: " << name << " ID: " << ID << " Parent ID: " << parentID << "\n";
			return ss.str();
		};

	};

	struct ProjectGroup : public GroupObject
	{
	public:
		ProjectGroup(uint32_t ID, const std::string& name, uint32_t parent_id)
			:GroupObject(ID, name, parent_id) {};
		ProjectGroup(const std::string& name, uint32_t parent_id)
			:GroupObject(name, parent_id) {};
		ProjectGroup() = default;

		std::string toString() const override {
			std::stringstream ss;
			ss << "Name: " << name << "ID: " << ID << "Parent ID: " << parentID << "\n";
			return ss.str();
		};

	};
	struct Color {
		float r;
		float g;
		float b;
		float a;

		Color() = default;

		std::string toString() const {
			std::stringstream ss;
			ss << "(" << r << "," << g << "," << b << "," << a << ")";
			return ss.str();
		}
	};


	struct Tag : public BaseObject {
		Color tag_color;
		int tag_group_id;

		Tag(uint32_t ID, const std::string& name, Color color, int taggp_id)
			:BaseObject(ID, name), tag_color(color), tag_group_id(taggp_id) {};

		Tag( const std::string& name, Color color, int taggp_id)
			:BaseObject(name), tag_color(color), tag_group_id(taggp_id) {};

		Tag() = default;

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
		DateTime start_time;
		DateTime end_time;
		uint32_t project_id;
		std::vector<std::shared_ptr<Tag>> tag_list;

		Session() = default;

		Session(uint32_t ID, const std::string& name, const std::string& discription, DateTime start_time, DateTime end_time, float duration, uint32_t project_ID)
			:BaseObject(ID, name),
			discription(discription),
			start_time(start_time),
			end_time(end_time), 
			duration(duration), 
			project_id(project_ID),
			tag_list(std::vector<std::shared_ptr<Tag>>())
		{};

		Session(const std::string& name, const std::string discription, DateTime start_time, DateTime end_time, float duration, uint32_t project_ID = 0) 
			:BaseObject(name),
			discription(discription),
			start_time(start_time),
			end_time(end_time),
			duration(duration),
			project_id(project_ID),
			tag_list(std::vector<std::shared_ptr<Tag>>())
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
			:BaseObject(ID, name), project_color(color), discription(discription), project_group_id(prjgp_id), tag_list(std::vector<std::shared_ptr<Tag>>()) {};

		Project(const std::string& name, const std::string& discription, Color color, int prjgp_id)
			:BaseObject(name), project_color(color), discription(discription), project_group_id(prjgp_id), tag_list(std::vector<std::shared_ptr<Tag>>()) {};
	
		Project() = default;

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