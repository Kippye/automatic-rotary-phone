workspace "ARP"
	configurations { "Debug", "Release" }
	startproject "ARP"
	
project "ARP"
	location ""
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	--architecture "x86_64"
	
	targetdir "."
	objdir "obj/%{cfg.buildcfg}"
	
	files 
	{ 
		"source/**.cpp",
		"include/*.h"
	}
	
	includedirs 
	{ 
		"include/", 
		"include/glad/include", 
		"include/glad/include/glad",
		"include/glfw/include", 
		"include/glm",
		"include/imgui",
		"include/glew",
		"include/blf/include",
		"include/blf/include/blf",
		"include/ImGuiFileDialog-Lib_Only",
		"include/libconfig/lib"
	}
	
	defines
	{
		"GLFW_INCLUDE_NONE"
	}
	
	links
	{
		"glad",
		"glfw",
		"imgui",
		"blf",
		"libconfig++"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

group "Dependencies"

project "glad"
	location "include/glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"
	
	targetdir "bin/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"
	
	files 
	{ 
		"%{prj.location}/src/glad.c",
		"%{prj.location}/include/KHR/khrplatform.h"
	}
	
	includedirs 
	{ 
		"%{prj.location}/include",
		"%{prj.location}/include/KHR"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		

externalproject("glfw")
	location "include/glfw"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir "bin/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"

project "imgui"
	location "libs/imgui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir "bin/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"
	
	files 
	{ 
		"%{prj.location}/*.cpp"
	}
	
	includedirs
	{ 
		"../../include/imgui/",
		"include/imgui",
		"include/glad/include",
		"include/glfw/include/" 
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		
project("blf")
	location "include/blf"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	files
	{
		"%{prj.location}/src/blf/**.cpp"
	}
	
	includedirs
	{
		"%{prj.location}/include",
		"%{prj.location}/include/blf"
	}
	
	targetdir "bin/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		
externalproject("libconfig++")
	location "include/libconfig/lib"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir "bin/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"