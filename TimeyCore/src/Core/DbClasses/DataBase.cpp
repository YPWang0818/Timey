#include "timey_pch.h"
#include "DataBase.h"
#include "tuple"

namespace Timey {

	UniqueCol u;
	CostumCol d("bbb");
	ColRange r(2.3, 3.4);
	
	Column<ColumnType::Integer> col("test", u, d, r);
	const const std::string name1 = d.value;
	const const char* name2 = u.value;
	int up = r.Upper; int lw = r.Lower;



}