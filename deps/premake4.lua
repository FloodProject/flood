-- This is the starting point of the build scripts for the project.
-- It defines the common build settings that all the projects share
-- and calls the build scripts of all the sub-projects.

action = _ACTION or ""

common_flags = { "Unicode", "Symbols", "NoExceptions", "NoRTTI" }
msvc_buildflags = { "/wd4190", "/wd4996", "/wd4530" }
gcc_buildflags = { "-Wno-invalid-offsetof", "-std=gnu++11" }

solution "Dependencies"

	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }
	
	flags { common_flags }
	language "C++"
	
	local build = "build/" .. action
	
	location (build)
	objdir (build .. "/obj/")
	targetdir (build .. "/lib/")
	
	-- Build configuration options
	
	configuration "Debug"
		defines { "DEBUG", "_DEBUG" }
		flags { "NoMinimalRebuild", "FloatFast", "NoEditAndContinue" }
		targetsuffix "_d"	
	
	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize", "NoMinimalRebuild", "FloatFast" }	
	
	-- Compiler-specific options
	
	configuration "vs*"
		buildoptions { msvc_buildflags }
	
	configuration "gcc"
		buildoptions { gcc_buildflags }
	
	-- OS-specific options
	
	configuration "Windows"
		defines { "WIN32", "_WINDOWS" }
		
	configuration "vs*"
    defines { "_CRT_SECURE_NO_WARNINGS" }
		
	configuration {}
	
	--function IncludeDependencies() do
		print("Searching for dependencies...")
		local deps = os.matchdirs("*")
		
		for i,dep in ipairs(deps) do
			local fp = path.join(dep, "premake4.lua")
			fp = path.join(os.getcwd(), fp)
			
			if os.isfile(fp) then
				print(string.format(" including %s", dep))
				include(dep)
			end
		end
	--end