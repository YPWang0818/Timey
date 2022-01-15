cmake_minimum_required(VERSION 3.20)
project(app)

set (source
	"src/timey_pch.cpp"
	"src/timey_pch.h"
	"src/AppCore/Application.cpp"
	"src/AppCore/Application.h"
	"src/AppCore/BaseWindow.cpp"
	"src/AppCore/BaseWindow.h"
	"src/AppCore/main.cpp"
	"src/Events/ApplicationEvents.h"
	"src/Events/Event.h"
	"src/Events/KeyEvents.h"
	"src/Events/MouseEvents.h"
	"src/Events/TimeyEvents.h"
	"src/Events/WindowEvents.h"
	"src/Platform/Win32/timey_pch.cpp"
	"src/Platform/Win32/Win32Window.cpp"
	"src/Platform/Win32/Win32Windows.h"
	"src/UI/ImguiBackend.cpp"
	"src/UI/ImguiBackend.h"
	"src/UI/UILayer.cpp"
	"src/UI/UILayer.h"
	"src/UI/UIWindows.cpp"
	"src/UI/UIWindows.h"
	)

	

add_executable(timey_gui ${source})
target_link_libraries(timey_gui 
					  PUBLIC timey_core)

#add_subdirectory(test)

