-- This is the starting point of the build scripts for the project.
-- It defines the common build settings that all the projects share
-- and calls the build scripts of all the sub-projects.

action = _ACTION or ""
common_flags = { "Unicode", "NoExceptions", "NoRTTI", "Symbols" }
common_msvc_copts = { "/wd4190", "/wd4530" }

solution "vapor"

	configurations { 
		"Debug",
		"Release"
	}
	
	configuration "gcc"
		buildoptions "-Wno-invalid-offsetof"	
	
	configuration { "not macosx", "gcc" }
		buildoptions "-std=gnu++0x"
		
	configuration "vs2010"
		buildoptions { common_msvc_copts }
		
	configuration "Windows"
		defines { "WIN32", "_WINDOWS" }
		
	configuration "Debug"
		defines { "DEBUG" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }

	location (action)
	objdir (action .. "/obj")
	targetdir (action .. "/lib")
	
	language "C++"
	
	dofile "Core.lua"
	dofile "Resources.lua"
	dofile "Engine.lua"
	dofile "Pipeline.lua"
	dofile "Editor.lua"