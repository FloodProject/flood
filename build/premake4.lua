-- This is the starting point of the build scripts for the project.
-- It defines the common build settings that all the projects share
-- and calls the build scripts of all the sub-projects.

action = _ACTION or ""
common_flags = { "Unicode", "Symbols", "NoExceptions", "NoRTTI" }
common_msvc_copts = { "/wd4190", "/wd4530" }

-- Common Libraries

Mono = {}
Mono.links = { "eglib", "libgc", "mono-2.0" }

wxWidgets = {}
wxWidgets.links = {}

solution "Flush"

	configurations
	{ 
		"Debug",
		"Release"
	}
	
	startup "Editor"
	
	language "C++"
	
	location (action)
	objdir (action .. "/obj/")
	targetdir (action .. "/lib/")
	
	-- Build configuration options
	
	configuration "Debug"
		defines { "DEBUG" }
		targetsuffix "_d"
	
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
	dofile "EngineManaged.lua"
	dofile "Pipeline.lua"
	dofile "Editor.lua"
	dofile "EditorManaged.lua"
	dofile "Runtime.lua"
	
-- Copy a configuration build header if one does not exist yet.

if not os.isfile("Build.h") then
	os.copyfile("Build0.h", "Build.h")
end


