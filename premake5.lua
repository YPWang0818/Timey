workspace "Timey"
	architecture "x64"
	
	configurations{
		"Debug",
		"Release",
		"Dist"
	}
	
	IncludeDir = {}
	IncludeDir["sqlite3"] = "vendor/sqlite3"
	
	
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
		
		
		files {
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.hpp",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/src/**.c"
		
		}
		
			
		includedirs {
			"TimeyCore/src",
			"%{IncludeDir.sqlite3}"
			}
			
		links{	"TimeyCore"
		}
		
		filter {"system:windows"}
			staticruntime "on"
			systemversion "latest"
			
			defines{
			"TIMEY_PLATFORM_WINDOWS"
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
		cppdialect "C++17"
	
		targetdir ("bin/" .. outdir .. "/%{prj.name}")
		objdir ("obj/" .. outdir .. "/%{prj.name}")
		
		
		files {
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.hpp",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/src/**.c"
		
		}
		
		
			
		includedirs {
			"%{IncludeDir.sqlite3}"
		}
		
		links{	"sqlite3"
		}
		
		filter {"system:windows"}
			staticruntime "on"
			systemversion "latest"
			
			defines{
			"TIMEY_PLATFORM_WINDOWS"
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
	group ""