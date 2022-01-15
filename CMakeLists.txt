cmake_minimum_required(VERSION 3.20)

project(TIMEY
        VERSION 0.0.1
		LANGUAGES C CXX)
		


# The include directories for modules. 
set(moduledirs
	"${CMAKE_SOURCE_DIR}/vendor/sqlite3"
	"${CMAKE_SOURCE_DIR}/vendor/spdlog/include"
)

# The precompiled header path
set( precomppath
	"${CMAKE_SOURCE_DIR}/TimeyCore/src/timey_pch.h"
)

if(WIN32)
	add_compile_definitions(TIMEY_PLATFORM_WINDOWS)
endif()
# TODO: Port to other operating systems.

add_subdirectory(vendor/sqlite3)
add_subdirectory(vendor/spdlog)		
add_subdirectory(TimeyCore)		
add_subdirectory(TimeyApplication)
