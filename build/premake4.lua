-- This is the starting point of the build scripts for the project.
-- It defines the common build settings that all the projects share
-- and calls the build scripts of all the sub-projects.

action = _ACTION or ""

common_flags = { "Unicode", "Symbols", "NoExceptions", "NoRTTI", "ParallelBuild" }
msvc_buildflags = { "/wd4190", "/wd4530", "/wd4251" }
gcc_buildflags = { "-Wno-invalid-offsetof", "-std=gnu++11" }

dofile "Helpers.lua"

-- Copy a configuration build header if one does not exist yet.

if not os.isfile("Config.lua") then
	print("Build configuration file 'Config.lua' created")
	os.copyfile("Config0.lua", "Config.lua")
end

dofile "Config.lua"

print("Generating the build configuration 'Build.h'")
local conf = GenerateBuildConfig(config)
WriteConfigToFile(conf, "Build.h")

solution "Flush"
	
	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }
	defaultplatform "x32"
	
	flags { common_flags }
	language "C++"
	framework "4.5"
	
	local build = action
	location (build)
	objdir (build .. "/obj/")
	targetdir (build .. "/lib/")
	libdirs { build .. "/lib/" }
	
	debugdir "../bin"
	startup "Editor"

	-- Build configuration options
	
	configuration "Debug"
		defines { "DEBUG" }
		targetsuffix "_d"
	
	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }	

	-- Compiler-specific options
	
	configuration "vs*"
		buildoptions { msvc_buildflags }
	
	configuration "gcc"
		buildoptions { gcc_buildflags }
	
	-- OS-specific options
	
	configuration "Windows"
		defines { "WIN32", "_WINDOWS" }
		
	configuration {}

	group "Engines"
	
		dofile "Core.lua"
		dofile "Resources.lua"
		dofile "Graphics.lua"
		dofile "Engine.lua"
		dofile "Pipeline.lua"

	group "Runtimes"

		dofile "Runtime.lua"
		dofile "GameRuntime.lua"

	group "Managed"

		dofile "EngineManaged.lua"
		dofile "GUI.lua"
		dofile "EditorManaged.lua"	
		dofile "Editor.lua"

	group "Tests"

		dofile "UnitTests.lua"

	group "Examples"
