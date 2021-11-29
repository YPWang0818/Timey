#include "timey_pch.h"
#include "sqlite3.h"
#include"DataBase.h"
#include "SQL_queries.h"



namespace Timey {


	void SqliteDb::initDb(const std::string& filePath)
	{	
		namespace fs = std::filesystem;
		if (!fs::is_regular_file(filePath))
		{
			// Database not found, create a new one. 

			int ok = sqlite3_open_v2(filePath.c_str(), &sqliteHandle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
			if (ok != SQLITE_OK) {
				TIMEY_ERROR("[Sqlite3] Failed to open file {0}", filePath.c_str());
				const char* msg = sqlite3_errmsg(sqliteHandle);
				TIMEY_CORE_ERROR("[Sqlite3] {0}", msg);
			}
		};
	

		int ok = sqlite3_open_v2(filePath.c_str(), &sqliteHandle, SQLITE_OPEN_READWRITE, nullptr);
		if (ok != SQLITE_OK) {
			TIMEY_ERROR("[Sqlite3] Failed to open file {0}", filePath.c_str());
			const char* msg = sqlite3_errmsg(sqliteHandle);
			TIMEY_CORE_ERROR("[Sqlite3] {0}", msg);
		}
	}

	void SqliteDb::closeDb()
	{
		sqlite3_close(sqliteHandle);
	}

	int SqliteQuery::compile(const SqliteDb& db)
	{
		int ok = sqlite3_prepare_v2(db.getRawDbHanle(), query.c_str(), -1, &stmt, nullptr);
		if (ok != SQLITE_OK) {
			TIMEY_CORE_ERROR("[Sqlite3] stmt compile faliure {0}", ok);
			const char* msg = sqlite3_errmsg(db.getRawDbHanle());
			TIMEY_CORE_ASSERT(false, msg);
		};

		prepared = true;
		bindedDb = &db;

		return ok;
	}

	void SqliteQuery::detach()
	{
		unbindAllColumns();
		sqlite3_finalize(stmt);
		stmt = nullptr;
		bindedDb = nullptr;
		prepared = false;

	}

	int SqliteQuery::bindColumnInteger(uint32_t idx, int value)
	{
		if (!prepared) { TIMEY_ASSERT(false, "The statement is not prepared."); };

	
		int ok = sqlite3_bind_int(stmt, idx, value);
		bdErrorMsg(ok);

		return ok;
	}

	int SqliteQuery::bindColumnReal(uint32_t idx, double value)
	{
		if (!prepared) { TIMEY_ASSERT(false, "The statement is not prepared."); };

	
		int ok = sqlite3_bind_double(stmt, idx, value);
		bdErrorMsg(ok);

		return ok;
	}

	int SqliteQuery::bindColumnText(uint32_t idx, const char* value)
	{
		if (!prepared) { TIMEY_ASSERT(false, "The statement is not prepared."); };

		int ok = sqlite3_bind_text(stmt, idx, value, -1, nullptr);
		bdErrorMsg(ok);

		return ok;
	}

	int SqliteQuery::bindColumnText(uint32_t idx, const std::string& value)
	{
		return bindColumnText(idx, value.c_str());
	}

	int SqliteQuery::bindColumnNull(uint32_t idx)
	{
		if (!prepared) { TIMEY_ASSERT(false, "The statement is not prepared."); };

		int ok = sqlite3_bind_null(stmt, idx);
		bdErrorMsg(ok);

		return ok;
	}

	int SqliteQuery::bindColumnBlob(uint32_t idx, const void* data, uint32_t size)
	{
		if (!prepared) { TIMEY_ASSERT(false, "The statement is not prepared."); };


		int ok = sqlite3_bind_blob(stmt, idx, data, size, nullptr);
		bdErrorMsg(ok);

		return ok;
	}

	int SqliteQuery::unbindAllColumns()
	{
		sqlite3_reset(stmt);
		return sqlite3_clear_bindings(stmt);
	
	}

	Ref<SqliteTable> SqliteQuery::exec()
	{
		TIMEY_CORE_ASSERT(( bindedDb != nullptr), "No database binded.");
		
		Ref<SqliteTable> tb{ new SqliteTable(stmt) };
	
		return tb;
	}

	Ref<SqliteRow> SqliteTable::getCurrentRow()
	{

		//if (!currentRow) return nullptr;		
		if (cache.find(currentRow) != cache.end()) { return cache[currentRow]; };

		Ref<SqliteRow> row = CreateRef<SqliteRow>();
		row->reserve(colCount);

		for (int col = 0; col < colCount; col++) {

			//This switch statment should have explicit control over the type of SqliteEntry it creates.

			switch (SqliteTable::getTypeFromNativeType(sqlite3_column_type(stmt, col))) {
			case SQLType::integer: 
				row->emplace_back(getEntryInt(col)); 
				break;

			case SQLType::blob: 
				const void* data; std::size_t sz; 
				data = getEntryBlob(col, sz);
				row->emplace_back(data, sz);
				break;

			case SQLType::null:;
				row->emplace_back();
				break;

			case SQLType::real:
				row->emplace_back(getEntryReal(col));
				break;

			case SQLType::text:
				const char* cdata; std::size_t csz;
				cdata = getEntryText(col, csz);
				row->emplace_back(cdata, csz);
				break;

			default:
				TIMEY_CORE_ERROR("Undefined SQLType.");
				break;
			};
		}

		cache[currentRow] = row;

		return row;
	}

	int SqliteTable::nextRow()
	{
		int ok = sqlite3_step(stmt);
		currentRow++;

		if ((ok != SQLITE_ROW) && (ok != SQLITE_DONE))
		{
			TIMEY_CORE_ERROR("Execute query failure.({0})", ok);
		};

		return ok;
	};

	void SqliteTable::resetRow()
	{
		currentRow = 0;
		sqlite3_reset(stmt);

	};

	Ref<SqliteColumn> SqliteTable::getColumn(std::size_t col)
	{
		uint32_t currentRowSaved = currentRow;

		resetRow(); // reset to intitial row status. 

		Ref<SqliteColumn> column = CreateRef<SqliteColumn>();
		TIMEY_CORE_ASSERT(col < colCount, "Column out of bound.");

		while (nextRow() == SQLITE_ROW) {
			column->push_back((* (getCurrentRow()) )[col]);
		}

		for (int col = 0; col < currentRowSaved; col++)
		{
			sqlite3_step(stmt); // restore the row status. 		
		};

		currentRow = currentRowSaved;
		return column;

	}

	int64_t SqliteTable::getEntryInt(uint32_t col)
	{
		return sqlite3_column_int64(stmt, col);
	}

	double SqliteTable::getEntryReal(uint32_t col)
	{
		 return sqlite3_column_double(stmt, col);
	}

	const char* SqliteTable::getEntryText(uint32_t col, std::size_t& sz)
	{
		sz = sqlite3_column_bytes(stmt, col) + 1;
		return (const char*)(sqlite3_column_text(stmt, col));
	};

	const void* SqliteTable::getEntryBlob(uint32_t col, std::size_t& sz)
	{
		sz = sqlite3_column_bytes(stmt, col);
		return sqlite3_column_blob(stmt, col);
	}

	SQLType SqliteTable::getTypeFromNativeType(int type)
	{
		switch (type) {

		case SQLITE_INTEGER: return SQLType::integer;
		case SQLITE_FLOAT: return SQLType::real;
		case SQLITE_TEXT: return SQLType::text;
		case SQLITE_BLOB: return SQLType::blob;
		case SQLITE_NULL: return SQLType::null;
		default: return SQLType::undef;

		};
	}

	std::string SqliteTable::toString()
	{
		std::stringstream ss;

		while (nextRow() == SQLITE_ROW) {

			Ref<SqliteRow> row = getCurrentRow();
			for (auto& e : *row) {
				switch (e.type) {
				case SQLType::integer:
					ss << e.data.i << " | ";
					break;
				case SQLType::real:
					ss << e.data.d << " | ";
					break;
				case SQLType::null:
					ss << "NULL" << " | ";
					break;
				case SQLType::text:
					ss << e.data.c << " | ";
					break;
				case SQLType::blob:
					ss << "(Blob item)" << " | ";
					break;
				default:
					break;
				}
			};

			ss << " \n ";

		}

		resetRow();

		return ss.str();
	};



	void SqliteQuery::bdErrorMsg(int errcode)
	{
		if ((errcode != SQLITE_OK)) {
			const char* msg = sqlite3_errmsg(bindedDb->getRawDbHanle());
			TIMEY_CORE_ERROR("[Sqlite3] Binding Failure.\n");
			TIMEY_CORE_ASSERT(false, msg);
		};
	}




	template <typename DataType>
	void DataBase<DataType>::installQuery(const std::string& name, const SqliteQuery& query)
	{
		// TODO: handle collisions.
		queryCache[name] = query;
		queryCache[name].compile(*this);
	}

	template<typename DataType>
	Ref<SqliteTable> DataBase<DataType>::execQuery(const std::string& name)
	{
		auto res = queryCache.find(name);
		if ( res != queryCache.end() ) {
			return (res->second).exec();
		}
		else{
			TIMEY_ASSERT(false, "Can not find the query.");
			return nullptr;
		};
	
	}

	template<typename DataType>
	void DataBase<DataType>::tearDownDb()
	{
		for (auto query : queryCache) {
			(query.second).detach();
		};
	}


	int ProjectDb::insertData(const Project& data)
	{
		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryAddProject));
		for (int i = 0; i < 4; ++i) {
			query->bindColumnReal(i + 1, data.color[i]);
		};

		query->bindColumnText(5, data.name);
		query->bindColumnText(6, data.discription);
		query->bindColumnInteger(7, data.project_group_id);
		
		query->exec();
		query->unbindAllColumns();

		return 0;
	}

	int ProjectDb::updateData(const Project& data)
	{
		TIMEY_ASSERT(data.ID, "The data to be updated is not initialized in database.");

		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryUpdateProject));
		for (int i = 0; i < 4; ++i) {
			query->bindColumnReal(i + 1, data.color[i]);
		};

		query->bindColumnText(5, data.name);
		query->bindColumnText(6, data.discription);
		query->bindColumnInteger(7, data.project_group_id);
		query->bindColumnInteger(8, data.ID);

		query->exec();
		query->unbindAllColumns();
		return 0;
	}

	int ProjectDb::deleteData(uint32_t id)
	{
		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryDeleteProject));
		query->bindColumnInteger(1, id);
		query->exec();
		query->unbindAllColumns();

		return 0;
	}

	Ref<Project> ProjectDb::fetchData(uint32_t id)
	{
		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryFetchProject));
		query->bindColumnInteger(1, id);
		Ref<SqliteTable> result = query->exec();
	

		if (result->isEmpty())
		{
			TIMEY_CORE_WARN("Cannot find project with id {}.", id);
			return nullptr;
		};

		Ref<SqliteRow> row = result->getCurrentRow();
		Ref<Project> newProject = CreateRef<Project>();

		newProject->ID = (*row)[0];
		newProject->color = { (*row)[1] ,  (*row)[2] , (*row)[3] ,  (*row)[4] };
		newProject->name = (*row)[5];
		newProject->discription = (*row)[6];
		newProject->project_group_id = (*row)[7];


		query->unbindAllColumns();
		return newProject;
	}

	void ProjectDb::initProjectDb()
	{
		TIMEY_INSTALL_QUERY(queryCreateProjectsTable);
		execQuery(TIMEY_TO_STR(queryCreateProjectsTable));

		TIMEY_INSTALL_QUERY(queryAddProject);
		TIMEY_INSTALL_QUERY(queryFetchProject);
		TIMEY_INSTALL_QUERY(queryDeleteProject);
		TIMEY_INSTALL_QUERY(queryUpdateProject);


	}

	int SessionDb::insertData(const Session& data)
	{
		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryAddSession));
	
		query->bindColumnInteger(1, data.duration);
		query->bindColumnInteger(2, data.start_time);
		query->bindColumnInteger(3, data.end_time);
		query->bindColumnText(4, data.name);
		query->bindColumnText(5, data.discription);
		query->bindColumnInteger(6, data.project_id);

		query->exec();
		query->unbindAllColumns();
		return 0;
	}

	int SessionDb::updateData(const Session& data)
	{

		TIMEY_ASSERT(data.ID, "The data to be updated is not initialized in database.");
	

		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryUpdateSession));
	
		query->bindColumnInteger(1, data.duration);
		query->bindColumnInteger(2, data.start_time);
		query->bindColumnInteger(3, data.end_time);
		query->bindColumnText(4, data.name);
		query->bindColumnText(5, data.discription);
		query->bindColumnInteger(6, data.project_id);
		query->bindColumnInteger(7, data.ID);

		query->exec();
		query->unbindAllColumns();

		return 0;
	}

	int SessionDb::deleteData(uint32_t id)
	{
		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryDeleteSession));
		query->bindColumnInteger(1, id);
		query->exec();
		query->unbindAllColumns();

		return 0;
	}

	Ref<Session> SessionDb::fetchData(uint32_t id)
	{
		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryFetchSession));
		query->bindColumnInteger(1, id);
		Ref<SqliteTable> result = query->exec();


		if (result->isEmpty())
		{
			TIMEY_CORE_WARN("Cannot find session with id {}.", id);
			return nullptr;
		};

		Ref<SqliteRow> row = result->getCurrentRow();
		Ref<Session> newSession = CreateRef<Session>();

		newSession->ID = (*row)[0];
		newSession->duration = (*row)[1];
		newSession->start_time = (*row)[2];
		newSession->end_time = (*row)[3];
		newSession->name = (*row)[4];
		newSession->discription = (*row)[5];
		newSession->project_id = (*row)[6];
		query->unbindAllColumns();

		// Didn't fetch the tags.
		return newSession;
	}

	void SessionDb::initSessionDb()
	{

		TIMEY_INSTALL_QUERY(queryCreateSessionsTable);
		execQuery(TIMEY_TO_STR(queryCreateSessionsTable));

		TIMEY_INSTALL_QUERY(queryAddSession);
		TIMEY_INSTALL_QUERY(queryFetchSession);
		TIMEY_INSTALL_QUERY(queryDeleteSession);
		TIMEY_INSTALL_QUERY(queryUpdateSession);

	}


	int TagDb::insertData(const Tag& data)
	{
		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryAddTag));

		for (int i = 0; i < 4; ++i) {
			query->bindColumnReal(i + 1, data.color[i]);
		};

		query->bindColumnText(5, data.name);
		query->bindColumnInteger(6, data.tag_group_id);
		query->exec();
		query->unbindAllColumns();;
		return 0;
	}

	int TagDb::updateData(const Tag& data)
	{
		TIMEY_ASSERT(data.ID, "The data to be updated is not initialized in database.");

		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryUpdateTag));
		for (int i = 0; i < 4; ++i) {
			query->bindColumnReal(i + 1, data.color[i]);
		};

		query->bindColumnText(5, data.name);
		query->bindColumnInteger(6, data.tag_group_id);
		query->bindColumnInteger(7, data.ID);

		query->exec();
		query->unbindAllColumns();

		return 0;
	}

	int TagDb::deleteData(uint32_t id)
	{
		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryDeleteTag));
		query->bindColumnInteger(1, id);
		query->exec();
		query->unbindAllColumns();

		return 0;
	}

	Ref<Tag> TagDb::fetchData(uint32_t id)
	{
		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryFetchTag));
		query->bindColumnInteger(1, id);
		Ref<SqliteTable> result = query->exec();


		if (result->isEmpty())
		{
			TIMEY_CORE_WARN("Cannot find Tag with id {}.", id);
			return nullptr;
		};

		Ref<SqliteRow> row = result->getCurrentRow();
		Ref<Tag> newTag = CreateRef<Tag>();

		newTag->ID = (*row)[0];
		newTag->color = { (*row)[1] ,  (*row)[2] , (*row)[3] ,  (*row)[4] };
		newTag->name = (*row)[5];
		newTag->tag_group_id = (*row)[6];

		query->unbindAllColumns();
		
		return newTag;
	}

	void TagDb::initTagsDb()
	{
		TIMEY_INSTALL_QUERY(queryCreateTagsTable);
		execQuery(TIMEY_TO_STR(queryCreateTagsTable));

		TIMEY_INSTALL_QUERY(queryAddTag);
		TIMEY_INSTALL_QUERY(queryFetchTag);
		TIMEY_INSTALL_QUERY(queryDeleteTag);
		TIMEY_INSTALL_QUERY(queryUpdateTag);
	}


	int ProjectGroupDb::insertData(const ProjectGroup& data)
	{
		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryAddProjectGroup));

		query->bindColumnText(1, data.name);
		query->bindColumnInteger(2, data.parentID);

		query->exec();
		query->unbindAllColumns();

		return 0;
	}

	int ProjectGroupDb::updateData(const ProjectGroup& data)
	{

		TIMEY_ASSERT(data.ID, "The data to be updated is not initialized in database.");
		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryUpdateProjectGroup));

		query->bindColumnText(1, data.name);
		query->bindColumnInteger(2, data.parentID);
		query->bindColumnInteger(3, data.ID);

		query->exec();
		query->unbindAllColumns();

		return 0;
	}

	int ProjectGroupDb::deleteData(uint32_t id)
	{

		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryDeleteProjectGroup));
		query->bindColumnInteger(1, id);
		query->exec();
		query->unbindAllColumns();

		return 0;
	}

	Ref<ProjectGroup> ProjectGroupDb::fetchData(uint32_t id)
	{
		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryFetchProjectGroup));
		query->bindColumnInteger(1, id);
		Ref<SqliteTable> result = query->exec();


		if (result->isEmpty())
		{
			TIMEY_CORE_WARN("Cannot find project group with id {}.", id);
			return nullptr;
		};

		Ref<SqliteRow> row = result->getCurrentRow();
		Ref<ProjectGroup> newSession = CreateRef<ProjectGroup>();

		newSession->ID = (*row)[0];
		newSession->name = (*row)[1];
		newSession->parentID = (*row)[2];
		query->unbindAllColumns();

		// Didn't fetch the tags.
		return newSession;
	}

	void ProjectGroupDb::initProjectGroupDb()
	{
		TIMEY_INSTALL_QUERY(queryCreateProjectGroupsTable);
		execQuery(TIMEY_TO_STR(queryCreateProjectGroupsTable));

		TIMEY_INSTALL_QUERY(queryAddProjectGroup);
		TIMEY_INSTALL_QUERY(queryFetchProjectGroup);
		TIMEY_INSTALL_QUERY(queryDeleteProjectGroup);
		TIMEY_INSTALL_QUERY(queryUpdateProjectGroup);

	}


	int TagGroupDb::insertData(const TagGroup& data)
	{
		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryAddTagGroup));

		query->bindColumnText(1, data.name);
		query->bindColumnInteger(2, data.parentID);

		query->exec();
		query->unbindAllColumns();

		return 0;
	}


	int TagGroupDb::updateData(const TagGroup& data)
	{
		TIMEY_ASSERT(data.ID, "The data to be updated is not initialized in database.");
		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryUpdateTagGroup));

		query->bindColumnText(1, data.name);
		query->bindColumnInteger(2, data.parentID);
		query->bindColumnInteger(3, data.ID);

		query->exec();
		query->unbindAllColumns();

		return 0;
	}

	int TagGroupDb::deleteData(uint32_t id)
	{
		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryDeleteTagGroup));
		query->bindColumnInteger(1, id);
		query->exec();
		query->unbindAllColumns();

		return 0;
	}

	Ref<TagGroup> TagGroupDb::fetchData(uint32_t id)
	{

		Ref<SqliteQuery> query = getQuery(TIMEY_TO_STR(queryFetchTagGroup));
		query->bindColumnInteger(1, id);
		Ref<SqliteTable> result = query->exec();


		if (result->isEmpty())
		{
			TIMEY_CORE_WARN("Cannot find Tag Group with id {}.", id);
			return nullptr;
		};

		Ref<SqliteRow> row = result->getCurrentRow();
		Ref<TagGroup> newTagGroup = CreateRef<TagGroup>();

		newTagGroup->ID = (*row)[0];
		newTagGroup->name = (*row)[1];
		newTagGroup->parentID = (*row)[2];

		query->unbindAllColumns();

		return newTagGroup;
	}

	void TagGroupDb::initTagGroupDb()
	{
		TIMEY_INSTALL_QUERY(queryCreateTagGroupsTable);
		execQuery(TIMEY_TO_STR(queryCreateTagGroupsTable));

		TIMEY_INSTALL_QUERY(queryAddTagGroup);
		TIMEY_INSTALL_QUERY(queryFetchTagGroup);
		TIMEY_INSTALL_QUERY(queryDeleteTagGroup);
		TIMEY_INSTALL_QUERY(queryUpdateTagGroup);
	}

}