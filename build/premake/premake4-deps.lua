-- This is the starting point of the build scripts for the project.
-- It defines the common build settings that all the projects share
-- and calls the build scripts of all the sub-projects.

dofile "Helpers.lua"

ImportConfigs()

solution "Dependencies"

	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }
	defaultplatform "x32"
    
	flags { common_flags }
	language "C++"
	
	location (builddir)
	objdir (builddir .. "/obj/")
	targetdir (libdir)
	
    print("Searching for dependencies...")
	IncludePremakeProjects(path.join(depsdir,"*"))