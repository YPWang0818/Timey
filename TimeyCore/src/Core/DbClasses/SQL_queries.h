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
				 discription = ?6,
				 projects_group_id = ?7 
			WHERE  project_id = ?8;
		)";



	/// ********************************************************///
	///															///
	///					PROJECT GROUP QUERIES					///
	///															///
	/// ********************************************************///

	static const std::string queryCreateProjectGroupsTable = R"(
		-- project group

		CREATE TABLE projects_group (
			projects_group_id	INTEGER	PRIMARY KEY AUTOINCREMENT,
			name 				TEXT 	NOT NULL,
			parent_id			INTEGER,

			FOREIGN KEY (parent_id)  REFERENCES projects_group (projects_group_id) 
    			ON DELETE CASCADE
    			ON UPDATE CASCADE ); )";

	static const std::string queryAddProjectGroup = R"(
			INSERT INTO projects_group (name, parent_id) VALUES(?1, ?2);
		)";

	static const std::string queryDeleteProjectGroup = R"(
		DELETE FROM projects_group WHERE projects_group_id == ?1 ;
		)";

	static const std::string queryFetchProjectGroup = R"(
		SELECT * FROM projects_group WHERE projects_group_id == ?1;
		)";

	static const std::string queryUpdateProjectGroup = R"(
			UPDATE projects_group
			SET	 name = ?1,
				 parent_id = ?2
		    WHERE  projects_group_id = ?3; )";

	/// ********************************************************///
	///															///
	///						SESSION	 QUERIES					///
	///															///
	/// ********************************************************///


	static const std::string queryCreateSessionsTable = R"(
	-- create session table
	CREATE TABLE sessions (
		session_id		INTEGER		 PRIMARY KEY AUTOINCREMENT,
		duration		INTEGER 	NOT NULL,
		start_time		INTEGER		NOT NULL, 
		end_time		INTEGER		NOT NULL, 

		title			TEXT NOT NULL,
		discription		TEXT,
		project_id		INTEGER,

		FOREIGN KEY (project_id)  REFERENCES projects (project_id) 
    		ON DELETE RESTRICT
    		ON UPDATE CASCADE);
	)";


	static std::string queryAddSession = R"(INSERT INTO sessions (
			duration, 
			start_time,
			end_time, 
			title, 
			discription, 
			project_id)  
		VALUES(?1, ?2 ,?3, ?4, ?5, ?6 ); )";

	static const std::string queryFetchSession = R"(
		SELECT * FROM sessions WHERE session_id == ?1;
		)";

	static const std::string queryDeleteSession 
		= R"(
		DELETE FROM sessions WHERE session_id == ?1 ;
		)";

	static const std::string queryUpdateSession
		= R"(
			UPDATE sessions
			SET  duration = ?1,
				 start_time = ?2,
				 end_time = ?3,
				 title = ?4, 
				 discription = ?5,
				 project_id = ?6 
			WHERE  session_id = ?7;
		)";


	///*********************************************************///
	///															///
	///						TAG	 QUERIES						///
	///															///
	///*********************************************************///


	static const std::string queryCreateTagsTable = R"(
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


		static const std::string queryAddTag
			= R"( 
			INSERT INTO tags(color_r, color_g, color_b, color_a, title, tags_group_id)
			VALUES(?1, ?2, ?3, ?4, ?5, ?6); )";

		static const std::string queryFetchTag = R"( 
			SELECT * FROM tags WHERE tag_id == ?1; )";

		static const std::string queryDeleteTag = R"(
			DELETE FROM tags WHERE tag_id == ?1 ; )";

		static const std::string queryUpdateTag = R"(
			UPDATE tags
			SET  color_r = ?1,
				 color_g = ?2,
				 color_b = ?3,
				 color_a = ?4,
				 title = ?5,
				 tags_group_id = ?6
			WHERE  tag_id = ?7; )";


		///*********************************************************///
		///															///
		///						TAG	GROUP QUERIES					///
		///															///
		///*********************************************************///


		static const std::string queryCreateTagGroupsTable = R"(
		--tag group

		CREATE TABLE tags_group (
			tags_group_id	INTEGER	PRIMARY KEY AUTOINCREMENT,
			name 			TEXT	NOT NULL,
			parent_id		INTEGER,

			FOREIGN KEY (parent_id)  REFERENCES tags_group (tags_group_id) 
    			ON DELETE CASCADE
    			ON UPDATE CASCADE );
		)";

		static const std::string queryAddTagGroup = R"(
			INSERT INTO tags_group (name, parent_id) VALUES(?1, ?2);
		)";

		static const std::string queryDeleteTagGroup = R"(
		DELETE FROM tags_group WHERE tags_group_id == ?1 ;
		)";

		static const std::string queryFetchTagGroup = R"(
		SELECT * FROM tags_group WHERE tags_group_id == ?1;
		)";

		static const std::string queryUpdateTagGroup = R"(
			UPDATE tags_group
			SET	   name = ?1,
				   parent_id = ?2
		    WHERE  tags_group_id = ?3; )";


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