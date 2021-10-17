workspace "Timey"
	architecture "x64"
	
	configurations{
		"Debug",
		"Release",
		"Dist"
	}
	
	IncludeDir = {}
	IncludeDir["sqlite3"] = "vendor/sqlite3"
	IncludeDir["spdlog"] = "vendor/spdlog/include"
	IncludeDir["imgui"] = "vendor/imgui"
	IncludeDir["glad"] = "vendor/glad/include"
	IncludeDir["glfw"] = "vendor/glfw/include"
	
	outdir = "%{cfg.buildcfg}_%{cfg.architecture}_%{cfg.system}"
	workdir = "%{wks.location}"
	
	startproject "TimeyApplication"
	
	
	project "TimeyApplication"
		location "TimeyApplication"
		
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		
		targetdir ("bin/" .. outdir .. "/%{prj.name}")
		objdir ("obj/" .. outdir .. "/%{prj.name}")
		
		
		pchheader "timey_pch.h"
		pchsource "TimeyApplication/src/timey_pch.cpp"
		
		files {
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.hpp",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/src/**.c"
		
		}
		
			
		includedirs {
			"TimeyApplication/src",
			"TimeyApplication/src/AppCore",
			"TimeyCore/src",
			"%{IncludeDir.sqlite3}",
			"%{IncludeDir.spdlog}",
			"%{IncludeDir.imgui}",
			"%{IncludeDir.glad}",
			"%{IncludeDir.glfw}"
			}
			
		links{	"TimeyCore"
		}
		
		filter {"system:windows"}
			staticruntime "on"
			systemversion "latest"
			
			defines{
			"TIMEY_PLATFORM_WINDOWS",
			 "TIMEY_ENABLE_ASSERT"
			}		
			
		filter {"configurations:Debug"}
			defines{"TIMEY_DEBUG"}
			symbols "on"
			
		filter {"configurations:Release"}
			defines{"TIMEY_RELEASE"}
			optimize "on"
			
		filter {"configurations:Dist"}
			buildoptions "/MD"
			defines{"TIMEY_DIST"}
			optimize "speed"
		
		
		
	project "TimeyCore"
	
		location "TimeyCore"
		
		kind "StaticLib"
		language "C++"
		cppdialect "C++20"
	
		targetdir ("bin/" .. outdir .. "/%{prj.name}")
		objdir ("obj/" .. outdir .. "/%{prj.name}")
		
		pchheader "timey_pch.h"
		pchsource "TimeyCore/src/timey_pch.cpp"
		
		files {
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.hpp",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/src/**.c"
		}
		
		
			
		includedirs {
			"TimeyCore/src",
			"TimeyCore/src/Core",
			"%{IncludeDir.sqlite3}",
			"%{IncludeDir.spdlog}",
			"%{IncludeDir.imgui}",
			"%{IncludeDir.glad}",
			"%{IncludeDir.glfw}"
		}
		
		links{	
		"sqlite3",
		"imgui",
		"spdlog",
		"glad",
		"GLFW"
		}
		
		filter {"system:windows"}
			staticruntime "on"
			systemversion "latest"
			
			defines{
			"TIMEY_PLATFORM_WINDOWS",
			 "TIMEY_ENABLE_ASSERT"
			}	
			
			
		filter {"configurations:Debug"}
			defines{"TIMEY_DEBUG"}
			symbols "on"
			
		filter {"configurations:Release"}
			defines{"TIMEY_RELEASE"}
			optimize "on"
			
		filter {"configurations:Dist"}
			buildoptions "/MD"
			defines{"TIMEY_DIST"}
			optimize "speed"
	
	
	group "Dependencies"
		include "vendor/sqlite3"
		include "vendor/imgui"
		include "vendor/spdlog"
		include "vendor/glad"
		include "vendor/glfw"
	group ""