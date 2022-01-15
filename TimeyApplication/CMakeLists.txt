

set (source
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

	set(includedirs 
	"${CMAKE_SOURCE_DIR}/TimeyApplication/src/AppCore"
	"${CMAKE_SOURCE_DIR}/TimeyApplication/src"
	"${CMAKE_SOURCE_DIR}/TimeyCore/src"
	)

add_executable(timey_gui ${source} ${precomppath})

target_include_directories(timey_gui PRIVATE ${moduledirs} )
target_include_directories(timey_gui PRIVATE ${includedirs} )
target_compile_features(timey_gui PUBLIC cxx_std_20)

target_precompile_headers(timey_gui PUBLIC ${precomppath} )

target_link_libraries(timey_gui 
					  PUBLIC timey_core)

#add_subdirectory(test)

