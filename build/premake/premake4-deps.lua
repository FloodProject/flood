-- This is the starting point of the build scripts for the project.
-- It defines the common build settings that all the projects share
-- and calls the build scripts of all the sub-projects.

dofile "Helpers.lua"

solution "Dependencies"

	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }
	defaultplatform "x32"
    
	flags { common_flags }
	language "C++"
	
	location (builddir)
	objdir (builddir .. "/obj/")
	targetdir (libdir)
	
	function IncludeDependencies()
		print("Searching for dependencies...")
		local deps = os.matchdirs(depsdir.."/*")
		
		for i,dep in ipairs(deps) do
			local fp = path.join(dep, "premake4.lua")
			fp = path.join(os.getcwd(), fp)
			
			if os.isfile(fp) then
				print(string.format(" including %s", dep))
				include(dep)
			end
		end
	end

	IncludeDependencies()