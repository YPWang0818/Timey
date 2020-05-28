project "sqlite3"
	kind "StaticLib"
	language "C"

	targetdir (workdir .. "/bin/" .. outdir .. "/%{prj.name}")
	objdir (workdir .. "/obj/" .. outdir .. "/%{prj.name}")

	files
	{	"sqlite3.c",
		"sqlite3.h"
		}
	
	
	filter "system:windows"
		systemversion "latest"
		staticruntime "On"
		

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Dist"
		runtime "Release"
		optimize "speed"