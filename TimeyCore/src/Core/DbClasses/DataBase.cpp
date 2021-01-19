#include "timey_pch.h"
#include "DataBase.h"
#include "tuple"

namespace Timey {

	constexpr UniqueCol u;
	CostumCol<"Test">;


	using Hello_str = meta::stom_t<"Hello">;
	constexpr meta::string instance = ColumnStmt<"testColumn", ColumnType::Integer, UniqueCol, ColConstraint<"some constraint">>::value;



	using test = meta::strcat_t < "hello", "world", Hello_str{} >;


	 
}