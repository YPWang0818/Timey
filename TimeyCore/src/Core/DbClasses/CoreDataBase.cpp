#include "timey_pch.h"
#include"CoreDataBase.h"
#include"Query.h"
#include "FileManager.h"

namespace Timey {

	static const std::string create_sessionstb_query = R"(
-- create session table
CREATE TABLE sessions (
	session_id		INTEGER		 PRIMARY KEY AUTOINCREMENT,
	duration		REAL 	NOT NULL,

	start_year		INTEGER NOT NULL, 
	start_month		INTEGER NOT NULL CHECK( 1 <= start_month AND 12 >= start_month),
	start_day		INTEGER NOT NULL CHECK( 1 <= start_day AND 31 >= start_day),
	start_hour		INTEGER NOT NULL CHECK( 0 <= start_hour AND 24 >= start_hour),
	start_minute	INTEGER NOT NULL CHECK( 0 <= start_minute AND 60 > start_minute),
	start_second	INTEGER NOT NULL CHECK( 0 <= start_second AND 60 > start_second),

	end_year		INTEGER NOT NULL, 
	end_month		INTEGER NOT NULL CHECK( 1 <= end_month AND 12 >= end_month),
	end_day			INTEGER NOT NULL CHECK( 1 <= end_day AND 31 >= end_day),
	end_hour		INTEGER NOT NULL CHECK( 0 <= end_hour AND 24 >= end_hour),
	end_minute		INTEGER NOT NULL CHECK( 0 <= end_minute AND 60 > end_minute),
	end_second		INTEGER NOT NULL CHECK( 0 <= end_second AND 60 > end_second),

	title			TEXT NOT NULL,
	discription		TEXT,
	project_id		INTEGER,

	FOREIGN KEY (project_id)  REFERENCES projects (project_id) 
    	ON DELETE RESTRICT
    	ON UPDATE CASCADE);
)";

	static const std::string create_tagstb_query = R"(--tag table

CREATE TABLE tags (
	tag_id		INTEGER		 PRIMARY KEY AUTOINCREMENT,
	color_r 	REAL	NOT NULL CHECK (0 <= color_r AND 1 >= color_r),
	color_g		REAL  	NOT NULL CHECK (0 <= color_g AND 1 >= color_g),
	color_b 	REAL  	NOT NULL CHECK (0 <= color_b AND 1 >= color_b),
	color_a		REAL  	NOT NULL CHECK (0 <= color_a AND 1 >= color_a),
	title 		TEXT  	NOT NULL,
	tags_group_id INTEGER,

	FOREIGN KEY (tags_group_id)  REFERENCES tags_group (tags_group_id) 
    	ON DELETE RESTRICT
    	ON UPDATE CASCADE);
)";

	static const std::string create_projectstb_query = R"(
--project table


CREATE TABLE projects (
	project_id		INTEGER		 PRIMARY KEY AUTOINCREMENT,
	color_r 	REAL	NOT NULL CHECK (0 <= color_r AND 1 >= color_r),
	color_g		REAL	NOT NULL CHECK (0 <= color_g AND 1 >= color_g),
	color_b 	REAL	NOT NULL CHECK (0 <= color_b AND 1 >= color_b),
	color_a    	REAL	NOT NULL CHECK (0 <= color_a AND 1 >= color_a),
	title 		TEXT  	NOT NULL,
	discription 	TEXT,
	projects_group_id INTEGER,

	FOREIGN KEY (projects_group_id)  REFERENCES projects_group (projects_group_id) 
    	ON DELETE RESTRICT
    	ON UPDATE CASCADE );
)";

	static const std::string create_tagsgrouptb_query = R"(
--tag group

CREATE TABLE tags_group (
	tags_group_id	INTEGER	PRIMARY KEY AUTOINCREMENT,
	name 			TEXT	NOT NULL,
	parent_id		INTEGER,

	FOREIGN KEY (parent_id)  REFERENCES tags_group (tags_group_id) 
    	ON DELETE CASCADE
    	ON UPDATE CASCADE );
)";

	static const std::string create_projectsgrouptb_query = R"(
-- project group

CREATE TABLE projects_group (
	projects_group_id	INTEGER	PRIMARY KEY AUTOINCREMENT,
	name 				TEXT 	NOT NULL,
	parent_id			INTEGER,

	FOREIGN KEY (parent_id)  REFERENCES projects_group (projects_group_id) 
    	ON DELETE CASCADE
    	ON UPDATE CASCADE );
)";

	static const std::string create_tags_sessionstb_query = R"(
-- tag session relationship
CREATE TABLE tags_sessions (
	tag_id 		INTEGER 	NOT NULL,
	session_id 	INTEGER 	NOT NULL,
	PRIMARY KEY (tag_id, session_id),

	FOREIGN KEY (tag_id) REFERENCES tags (tag_id) 
        ON DELETE CASCADE
		ON UPDATE CASCADE ,
	FOREIGN KEY (session_id) REFERENCES sessions (session_id) 
        ON DELETE CASCADE 
		ON UPDATE CASCADE);
)";


	static const std::string create_tags_projectstb_query = R"(
-- tag project relationship
CREATE TABLE tags_projects (
	tag_id 		INTEGER 	NOT NULL,
	project_id 	INTEGER 	NOT NULL,
	PRIMARY KEY (tag_id, project_id),

	FOREIGN KEY (tag_id) REFERENCES tags (tag_id) 
        ON DELETE CASCADE
		ON UPDATE CASCADE ,
	FOREIGN KEY (project_id) REFERENCES projects (project_id) 
        ON DELETE CASCADE 
		ON UPDATE CASCADE);
)";

	CoreDataBase::CoreDataBase(const std::string& file_name)
		:_file_name(file_name)
	{

		FileManager& file_manager = FileManager::getFileManager();

		if (!file_manager.FindIfFileExists(_file_name)) {
			// Database not found, create a new one. 

			int ok = sqlite3_open_v2(_file_name.c_str(), &_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
			if (ok != SQLITE_OK) {
				TIMEY_ERROR("[Sqlite3] Failed to open file {0}", _file_name.c_str());
				const char* msg = sqlite3_errmsg(_db);
				TIMEY_CORE_ERROR("[Sqlite3] {0}", msg);
			}
			_create_data_base();
		}

		int ok = sqlite3_open_v2(_file_name.c_str(), &_db, SQLITE_OPEN_READWRITE, nullptr);
		if (ok != SQLITE_OK) {
			TIMEY_ERROR("[Sqlite3] Failed to open file {0}", _file_name.c_str());
			const char* msg = sqlite3_errmsg(_db);
			TIMEY_CORE_ERROR("[Sqlite3] {0}", msg);
		}


		
	}

	CoreDataBase::~CoreDataBase() {

		sqlite3_close(_db);

	}

	void CoreDataBase::AddQuery(const std::string& name, std::shared_ptr<Query> query)
	{
		if (_query_cache.find(name) != _query_cache.end()) {
			TIMEY_CORE_WARN("Overriding existing query. {0}", name.c_str());
		};

		_query_cache[name] = query ;
	}

	void CoreDataBase::RemoveQuery(const std::string& name)
	{
		if (_query_cache.find(name) == _query_cache.end() ) {

			TIMEY_CORE_WARN("Cannot find the query to be deleted.  {0}", name.c_str());
			return;
		}
		else {
			_query_cache.erase(name);
		}
	}

	void CoreDataBase::_create_data_base() {

		std::unique_ptr<Query> create_sessionstb = std::make_unique<Query>(create_sessionstb_query, this);
		std::unique_ptr<Query> create_tagstb = std::make_unique<Query>(create_tagstb_query, this);
		std::unique_ptr<Query> create_projectstb = std::make_unique<Query>(create_projectstb_query, this);
		std::unique_ptr<Query> create_tagsgrouptb = std::make_unique<Query>(create_tagsgrouptb_query, this);
		std::unique_ptr<Query> create_projectsgrouptb = std::make_unique<Query>(create_projectsgrouptb_query, this);
		std::unique_ptr<Query> create_tags_sessionstb = std::make_unique<Query>(create_tags_sessionstb_query, this);
		std::unique_ptr<Query> create_tags_projectstb = std::make_unique<Query>(create_tags_projectstb_query, this);

		create_sessionstb->ExecOnceNoRes();
		create_tagstb->ExecOnceNoRes();
		create_projectstb->ExecOnceNoRes();
		create_tagsgrouptb->ExecOnceNoRes();
		create_projectsgrouptb->ExecOnceNoRes();
		create_tags_sessionstb->ExecOnceNoRes();
		create_tags_projectstb->ExecOnceNoRes();

	};






}