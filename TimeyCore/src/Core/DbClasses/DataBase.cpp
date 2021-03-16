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


	using session_info_t = typename ComponetInfo<"Session", int , NotNull>;
	using color_info_t = typename ComponetInfo<"color_r", float, NotNull, CheckExpr<"0 <= color_b AND 1 >= color_b">>;
	
	


	static constexpr meta::string result = table_session::stmt;


	void print_res() {


		static int id = 822;
		static float color_r = 0.3f;

		Components<"SessionTb", color_info_t, session_info_t> comp(&color_r, &id);
		using comp_t = Components<"SessionTb", color_info_t, session_info_t>;

		constexpr int a = comp_t::findIdx<"Session">();
		using tp = comp_t::findType<"Session">;
		
		auto* res = comp.getData<"Session">();
		TIMEY_CORE_INFO( *res );
	}
	// TODO: Create a CompolmentInfo class that lives in Objects that are meant to be stored. Gives acces into each field of structure and manages the IDs and foreign keys. (Maybe as a inherentace?) 


}