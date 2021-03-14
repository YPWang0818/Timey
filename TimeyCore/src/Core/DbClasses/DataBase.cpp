#include "timey_pch.h"
#include "DataBase.h"
#include "tuple"
#include <typeinfo>

namespace Timey {


	using col_id = ColumnStmt<"session_id", ColumnType::Integer, PrimaryKey>;
	using col_projectsid = ColumnStmt<"project_id", ColumnType::Integer, NotNull>;
	using col_projectsids = ColNames< col_projectsid >;

	using col_color_r = ColumnStmt<"color_r", ColumnType::Real, NotNull, CheckExpr<"0 <= color_r AND 1 >= color_r"> >;
	using col_color_g = ColumnStmt<"color_g", ColumnType::Real, NotNull, CheckExpr<"0 <= color_g AND 1 >= color_g"> >;
	using col_color_b = ColumnStmt<"color_b", ColumnType::Real, NotNull, CheckExpr<"0 <= color_b AND 1 >= color_b"> >;

	using col_fgid = ColNames<ColumnStmt<"id", ColumnType::Integer, NotNull, PrimaryKey>>;


	using ky_Fgn = FgnKey<"projects",
		ForeignKeyCause::Cascade,
		ForeignKeyCause::Restrict,
		col_projectsids,
		col_fgid
		>;

	using table_session = TableStmt<"Session", void, ky_Fgn, col_id, col_projectsid, col_color_r, col_color_g, col_color_b >;
	
	int id = 8;
	float color_r = 0.3f;

	using session_info_t = ComponetInfo<"Session", int , NotNull>;
	using color_info_t = ComponetInfo<"color_r", float, NotNull, CheckExpr<"0 <= color_b AND 1 >= color_b">>;
	
	
	Components<session_info_t, color_info_t> comp(&id, &color_r);
	using comp_t = Components<color_info_t, session_info_t>;
	
	constexpr int a = comp_t::findIdx<"Session">();
	using tp = comp_t::findType<"Session">;

	static constexpr meta::string result = table_session::stmt;
 

	void print_res() {
		
		TIMEY_CORE_INFO(typeid(tp).name());
	}
	// TODO: Create a CompolmentInfo class that lives in Objects that are meant to be stored. Gives acces into each field of structure and manages the IDs and foreign keys. (Maybe as a inherentace?) 


}