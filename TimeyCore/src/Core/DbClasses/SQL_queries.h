#include <string>

#ifndef TIMEY_SQL_QUERIES
# define TIMEY_SQL_QUERIES
#endif

#ifdef  TIMEY_SQL_QUERIES

// Can only be used in the DataBase<DataType> class
#define TIMEY_INSTALL_QUERY(queryName) do{ \
	installQuery(#queryName, SqliteQuery{queryName});\
	}while(0);\

#define TIMEY_TO_STR(str) #str

#endif
namespace Timey {




	/// ********************************************************///
	///															///
	///						PROJECT	 QUERIES					///
	///															///
	/// ********************************************************///

	static const std::string queryCreateProjectsTable
		= R"(
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

	static const std::string queryAddProject
		= R"(INSERT INTO projects (color_r, color_g, color_b, color_a, title, discription, projects_group_id) 
			VALUES(?1, ?2, ?3, ?4, ?5, ?6, ?7); )";

	static const std::string queryFetchProject
		= R"( SELECT * FROM projects WHERE project_id == ?1; )";

	static const std::string queryDeleteProject 
		= R"(
		DELETE FROM projects WHERE project_id == ?1 ;
		)";

	static const std::string queryUpdateProject
		= R"(
			UPDATE projects
			SET  color_r = ?1,
				 color_g = ?2,
				 color_b = ?3,
				 color_a = ?4,
				 title = ?5, 
				 discription = ?6
			WHERE  project_id = ?7;
		)";

	static const std::string queryUpdateProjectGroup
		= R"(
			UPDATE projects
			SET  projects_group_id = ?1
			WHERE  project_id = ?2;
		)";
	


	/// ********************************************************///
	///															///
	///					PROJECT GROUP QUERIES					///
	///															///
	/// ********************************************************///

	static const std::string create_projectsgrouptb_query = R"(
		-- project group

		CREATE TABLE projects_group (
			projects_group_id	INTEGER	PRIMARY KEY AUTOINCREMENT,
			name 				TEXT 	NOT NULL,
			parent_id			INTEGER,

			FOREIGN KEY (parent_id)  REFERENCES projects_group (projects_group_id) 
    			ON DELETE CASCADE
    			ON UPDATE CASCADE ); )";

	static const std::string add_projects_group_query = R"(
			INSERT INTO projects_group (name, parent_id) VALUES(?1, ?2);
		)";

	static const std::string delete_projects_group_query = R"(
		DELETE FROM projects_group WHERE projects_group_id == ?1 ;
		)";

	static const std::string fetch_projects_group_query = R"(
		SELECT * FROM projects_group WHERE projects_group_id == ?1;
		)";


	/// ********************************************************///
	///															///
	///						SESSION	 QUERIES					///
	///															///
	/// ********************************************************///


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


	static std::string add_session_query
		= R"(INSERT INTO sessions (duration, start_year , start_month, start_day, start_hour, start_minute, start_second,
			 end_year, end_month, end_day, end_hour, end_minute, end_second, title, discription, project_id)  
			 VALUES(?1, ?2 ,?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, ?13, ?14, ?15, ?16);)";

	static const std::string fetch_session_query = R"(
		SELECT * FROM sessions WHERE session_id == ?1;
		)";

	static const std::string delete_session_query = R"(
		DELETE FROM sessions WHERE session_id == ?1 ;
		)";


	///*********************************************************///
	///															///
	///						TAG	 QUERIES						///
	///															///
	///*********************************************************///


	static const std::string create_tagstb_query = R"(
			--tag table
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
    				ON UPDATE CASCADE);	)";


		static const std::string add_tag_query
			= R"( 
			INSERT INTO tags(color_r, color_g, color_b, color_a, title, tags_group_id)
			VALUES(?1, ?2, ?3, ?4, ?5, ?6); )";

		static const std::string fetch_tag_query
			= R"( 
			SELECT * FROM tags WHERE tag_id == ?1; )";

		static const std::string delete_tag_query 
			= R"(
			DELETE FROM tags WHERE tag_id == ?1 ; )";

		static const std::string update_tags_group_query 
			= R"(
			UPDATE tags
			SET  tags_group_id = ?1
			WHERE  tag_id = ?2; )";


		///*********************************************************///
		///															///
		///						TAG	GROUP QUERIES					///
		///															///
		///*********************************************************///


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

		const std::string add_tags_group_query = R"(
			INSERT INTO tags_group (name, parent_id) VALUES(?1, ?2);
		)";

		static const std::string delete_tags_group_query = R"(
		DELETE FROM tags_group WHERE tags_group_id == ?1 ;
		)";

		static const std::string fetch_tags_group_query = R"(
		SELECT * FROM tags_group WHERE tags_group_id == ?1;
		)";


		/// ********************************************************///
		///															///
		///					CREATE TABLE QUERIES					///
		///															///
		/// ********************************************************///




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

}