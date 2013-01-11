Core = {}
Core.name = "Core"
Core.isShared = true
Core.defines = {}

project "Core"

	if Core.isShared then
		kind "SharedLib"
		table.insert(Core.defines, "API_CORE_DLL")
		defines { Core.defines, "API_CORE_DLL_EXPORT" }
	else
		kind "StaticLib"
	end
	
	SetupNativeProjects()

	pchheader "Core/API.h"
	pchsource "../src/Core/Core.cpp"

	files 
	{
		"Config.lua",
		"Core.lua",
		"../inc/Core/**.h",
		"../src/Core/**.cpp",
		"../inc/Core/Math/**.h",
		"../src/Core/Math/**.cpp",
		"../inc/Core/Network/**.h",
		"../src/Core/Network/**.cpp",
	}

	vpaths
	{
		["*"] = { "../src/Core/", "../inc/Core/" },
		["Platforms/*"] = { "../src/Platforms/" },
	}	

	excludes
	{
		"../src/Core/Test/**",
	}
	
	includedirs
	{
		"../inc/",
		"../deps/Dirent",
	}
	
	Core.links = {}
	
	Core.libdirs = {}

	Core.deps =
	{
		"cURL",
		"dlmalloc",
		"ENet",
		"FastLZ",
		"Jansson",
		"Mongoose",
		"zlib",
		"zziplib",
	}

	configuration "windows"
	
		files { "../src/Platforms/Win32/File*.cpp" }
		files { "../src/Platforms/Win32/Concurrency*.cpp" }
		
		table.insert(Core.links, "ws2_32")
		table.insert(Core.links, "winmm")
		
		-- Setup Visual Leak Detector
		if config.MEMORY_LEAK_DETECTOR then
			table.insert(Core.links, "vld")
			table.insert(Core.libdirs,
				depsdir .. "VisualLeakDetector/lib/Win32")
		end

	configuration {}

	links { Core.links }
	libdirs { Core.libdirs }
	deps(Core.deps)	

	