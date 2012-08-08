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

	vpaths
	{
		[""] = { "../src/", "../inc/", "../src/Core", "../inc/Core", },
		["Archive"] = "Archive*",
		["Compression"] = "fastlz*",
		["Concurrency"] = "Concurren*",
		["Cryptography"] = "Cipher*",
		["Debug"] = "StackWalker*",
		["Events"] = { "Delegate*", "Event*" },
		["Extensions"] = { "Plugin*", "Extension*" },
		["Libraries"] = "DynamicLibrary*",
		["Logs"] = "Log*",
		["Math"] = "Math*",
		["Memory"] = { "Handle*", "Memory*", "Pointers*", "References*" },
		["Network"] = { "Network*" },
		["Reflection"] = { "ClassWatcher*", "Object*", "Reflection*" },
		["Serialization"] = "Serialization*",
		["Serialization/Binary"] = "BinarySerializer*",
		["Serialization/JSON"] = "JsonSerializer*",
		["Streams"] = "Stream*",
		["Streams/File"] = "FileStream*",
		["Streams/Memory"] = "MemoryStream*",
		["Streams/Web"] = "WebStream*",
		["Streams/Watcher"] = "FileWatcher*",
		["Utilities"] = "Utilities*",
		["Utilities/String"] = { "String*", "Pattern*" },
		["Utilities/Profiler"] = "Profiler*",
		["Utilities/Timer"] = "Timer*",
		["Utilities/Telemetry"] = "Telemetry*",
	}
	
	includedirs
	{
		"../inc/",
		"../deps/Dirent",
		"../deps/VisualLeakDetector/include"
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
		if config.MEMORY_LEAK_DETECTOR then
			table.insert(Core.links, "vld")
			table.insert(Core.libdirs, "../deps/VisualLeakDetector/lib/Win32")
		end
	end
	
	deps(Core.deps)
	
	configuration "windows"
	
		files { "../src/Platforms/Win32/File*.cpp" }
		files { "../src/Platforms/Win32/Concurrency*.cpp" }
		