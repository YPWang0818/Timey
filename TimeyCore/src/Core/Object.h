#pragma once
#include "timey_pch.h"
#include "TimeyTime.h"

namespace Timey {

	// The abstract class for sessions, tags, projects, tags group, project group.

	struct BaseObject {
		std::string name;
		uint32_t ID = 0; 

		virtual std::string toString() const {
			std::stringstream ss;
			ss << "Name: " << name << " ID: " << ID;
			return ss.str();
		}
		
		bool operator==(const BaseObject& other) {
			bool isIDSame = (ID && other.ID) ? (ID == other.ID) : true; // Only compare the ID if both objects are initialized. 
			return isIDSame && (name == other.name);
		};

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

		bool operator==(const GroupObject& other) {
			return (static_cast<BaseObject>(*this) == static_cast<BaseObject>(other)) 
				&& (parentID == other.parentID);
		};

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

		//Color() = default;

		std::string toString() const {
			std::stringstream ss;
			ss << "(" << r << "," << g << "," << b << "," << a << ")";
			return ss.str();
		}

		bool operator==(const Color& other) {
				return (r = other.r) && (g == other.g) && (b == other.b) && (a == other.a);
		};
	};


	struct Tag : public BaseObject {
		
		union {
			std::array<float, 4> color;
			Color tag_color;
		};

		int tag_group_id;

		Tag(uint32_t ID, const std::string& name, Color color, int taggp_id)
			:BaseObject(ID, name), tag_color(color), tag_group_id(taggp_id) {};

		Tag( const std::string& name, Color color, int taggp_id)
			:BaseObject(name), tag_color(color), tag_group_id(taggp_id) {};

		Tag() = default;

		bool operator==(const Tag& other) {
			return (static_cast<BaseObject>(*this) == static_cast<BaseObject>(other)) &&
				(tag_group_id == other.tag_group_id) && (tag_color == other.tag_color);
		};
		virtual std::string toString() const  override {

			std::stringstream ss;
			ss << "Name: " << name << "\n";
			ss << "ID: " << ID << "\n";
			ss << "Color :" << tag_color.toString() << "\n";
			ss << "Tag Group ID: " << tag_group_id << "\n";

			return ss.str();
		}
	};


	using EpochTime = uint64_t;

	struct Session : public BaseObject
	{
		std::string discription;
		uint64_t duration;
		EpochTime start_time;
		EpochTime end_time;
		uint32_t project_id;
		std::vector<std::shared_ptr<Tag>> tag_list;

		Session() = default;

		Session(
			const std::string& name, 
			const std::string& discription,
			EpochTime start_time,
			EpochTime end_time,
			uint64_t duration, 
			uint32_t project_ID = 0,
			uint32_t ID = 0
		)
			:BaseObject(ID, name),
			discription(discription),
			start_time(start_time),
			end_time(end_time), 
			duration(duration), 
			project_id(project_ID),
			tag_list(std::vector<std::shared_ptr<Tag>>())
		{};

		// project_ID = 0 implies the session is in no projects.


		virtual std::string toString() const override {
			
			std::stringstream ss;

			ss << "Name: " << name << "\n";
			ss << "ID: " << ID << "\n";
			ss << "Duration: " << duration << "\n";
			ss << "Start time (Epoch Time): " << start_time << "\n";
			ss << "End time (Epcoh Time): " << end_time << "\n";
			ss << "Discription: " << discription << "\n";
			ss << "Project ID:" << project_id << "\n";

			ss << "Tag Ids: \n";

			for (auto tag : tag_list) {
				ss << tag->ID << ", ";
 			}

			return ss.str();
		}

		bool operator==(const Session& other) {

			bool success = true;
			if (tag_list.size() != other.tag_list.size()) {
				success = false;
			}
			else {
				for (int i = 0; i < tag_list.size(); i++) {
					success = success && (tag_list[i] == other.tag_list[i]);
				};
			};

			return (static_cast<BaseObject>(*this) == static_cast<BaseObject>(other)) &&
				(discription == other.discription) &&
				(duration == other.duration) &&
				(project_id == other.project_id) &&
				(start_time == other.start_time) &&
				(end_time == other.end_time) &&
				success;
		};

	};

	struct Project : public BaseObject
	{
		std::string discription;
		union
		{
			Color project_color;
			std::array<float, 4> color;
		};
		int project_group_id;
		std::vector<std::shared_ptr<Tag>> tag_list;


		bool operator==(const Project& other) {
			
			bool success = true;
			if (tag_list.size() == other.tag_list.size()) {
				success = true;
			}
			else {
				for (int i = 0; i < tag_list.size(); i++) {
					success = success && (tag_list[i] == other.tag_list[i]);
				};
			};
			
			return (static_cast<BaseObject>(*this) == static_cast<BaseObject>(other)) && 
				(discription == other.discription) &&
				(project_color == other.project_color) && 
				(project_group_id == other.project_group_id) && 
				success;
		};

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