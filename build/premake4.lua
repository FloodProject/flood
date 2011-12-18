-- This is the starting point of the build scripts for the project.
-- It defines the common build settings that all the projects share
-- and calls the build scripts of all the sub-projects.

action = _ACTION or ""
common_flags = { "Unicode", "Symbols", "NoExceptions", "NoRTTI" }
common_msvc_copts = { "/wd4190", "/wd4530" }

solution "vapor"

	configurations
	{ 
		"Debug",
		"Release"
	}
	
	language "C++"
	
	location (action)
	objdir (action .. "/obj")
	targetdir (action .. "/lib")	
	
	-- Build configuration options
	
	configuration "Debug"
		defines { "DEBUG" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }	

	-- Compiler-specific options
	
	configuration "vs*"
		buildoptions { common_msvc_copts }	
	
	configuration "gcc"
		buildoptions "-Wno-invalid-offsetof"	
		buildoptions "-std=gnu++0x"
	
	-- OS-specific options
	
	configuration "Windows"
		defines { "WIN32", "_WINDOWS" }
		
	configuration "*"
	
	dofile "Core.lua"
	dofile "Resources.lua"
	dofile "Graphics.lua"
	dofile "Engine.lua"
	dofile "Pipeline.lua"
	dofile "Editor.lua"
	
-- Copy a configuration build header if one does not exist yet.

if not os.isfile("Build.h") then
	os.copyfile("Build0.h", "Build.h")
end


