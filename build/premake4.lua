-- This is the starting point of the build scripts for the project.
-- It defines the common build settings that all the projects share
-- and calls the build scripts of all the sub-projects.

action = _ACTION or ""
flags_common = { "Unicode", "Symbols", "NoExceptions", "NoRTTI" }
flags_msvc = { "/wd4190", "/wd4530" }

dofile "Helpers.lua"

-- Copy a configuration build header if one does not exist yet.

if not os.isfile("Config.lua") then
	print("Build configuration file 'Config.lua' created")
	os.copyfile("Config0.lua", "Config.lua")
end

dofile "Config.lua"

FindWxWidgets()
FindFBX()
FindMono()

print("Generating the build configuration 'Build.h'")
local conf = GenerateBuildConfig(config)
WriteConfigToFile(conf, "Build.h")

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
	
	flags { flags_common }
		
	-- Build configuration options
	
	configuration "Debug"
		defines { "DEBUG" }
		targetsuffix "_d"
	
	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }	

	-- Compiler-specific options
	
	configuration "vs*"
		buildoptions { flags_msvc }	
	
	configuration "gcc"
		buildoptions "-Wno-invalid-offsetof"	
		buildoptions "-std=gnu++0x"
	
	-- OS-specific options
	
	configuration "Windows"
		defines { "WIN32", "_WINDOWS" }
		
	configuration {}
	
	dofile "Core.lua"
	dofile "Resources.lua"
	dofile "Graphics.lua"
	dofile "Engine.lua"
	dofile "Pipeline.lua"
	dofile "Editor.lua"
	dofile "Runtime.lua"
	dofile "UnitTests.lua"
	
	-- Keep the managed layer disabled for now.
	--dofile "EngineManaged.lua"
	--dofile "EditorManaged.lua"
