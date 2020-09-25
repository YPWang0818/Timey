project "glad"
	kind "StaticLib"
	language "C"
	cppdialect "C++17"
	staticruntime "On"
			
	targetdir (workdir .. "/bin/" .. outdir .. "/%{prj.name}")
	objdir (workdir .. "/obj/" .. outdir .. "/%{prj.name}")

	files
	{	"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
		}
	
	

	filter "system:windows"
		systemversion "latest"
		
		includedirs { "include" }

		defines 
		{ 
			"GLFW_INCLUDE_NONE"
		}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Dist"
		runtime "Release"
		optimize "speed"