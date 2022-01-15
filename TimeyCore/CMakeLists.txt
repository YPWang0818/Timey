
set (source
	"src/TimeyCore.h"
	"src/Core/FileManager.cpp"
	"src/Core/FileManager.h"
	"src/Core/Log.cpp"
	"src/Core/Log.h"
	"src/Core/Object.h"
	"src/Core/TimeyID.cpp"
	"src/Core/TimeyID.h"
	"src/Core/TimeyTime.cpp"
	"src/Core/TimeyTime.h"
	"src/Core/Utilty.h"
	"src/Core/DbClasses/DataBase.cpp"
	"src/Core/DbClasses/DataBase.h"
	"src/Core/DbClasses/SQL_queries.h"
	)


set(includedirs 
	"${CMAKE_SOURCE_DIR}/TimeyCore/src"
	"${CMAKE_SOURCE_DIR}/TimeyCore/src/Core" )
	
add_library(timey_core ${source} ${precomppath})
target_include_directories(timey_core PRIVATE ${moduledirs} )
target_include_directories(timey_core PRIVATE ${includedirs} )
target_compile_features(timey_core PUBLIC cxx_std_20)

target_precompile_headers(timey_core PUBLIC ${precomppath} )

target_link_libraries(timey_core 
					  PUBLIC sqlite3 spdlog)

