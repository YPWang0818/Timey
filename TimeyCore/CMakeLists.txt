cmake_minimum_required(VERSION 3.20)
project(core)

set (source
	"src/TimeyCore.h"
	"src/timey_pch.cpp"
	"src/timey_pch.h"
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

	

add_library(timey_core ${source})