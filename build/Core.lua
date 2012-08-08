Core = {}

project "Core"

	kind "StaticLib"
	pchheader "Core/API.h"
	pchsource "../src/Core/Core.cpp"

	files 
	{
		"Config.lua",
		"Core.lua",
		"../inc/Core/**.h",
		"../src/Core/**.cpp",
		"../inc/Math/**.h",
		"../src/Math/**.cpp",
		"../inc/Network/**.h",
		"../src/Network/**.cpp",
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
		"ENet",
		"FastLZ",
		"Jansson",
		"Mongoose",
		"zlib",
		"zziplib",
		"UnitTest++",
	}
	
	if os.is("windows") then
		table.insert(Core.links, "ws2_32")
		
		-- Setup Visual Leak Detector
--		if config.MEMORY_LEAK_DETECTOR then
--			table.insert(Core.links, "vld")
--			table.insert(Core.libdirs, "../deps/VisualLeakDetector/lib/Win32")
--		end
	end
	
	deps(Core.deps)
	
	configuration "windows"
	
		files { "../src/Platforms/Win32/File*.cpp" }
		files { "../src/Platforms/Win32/Concurrency*.cpp" }
		