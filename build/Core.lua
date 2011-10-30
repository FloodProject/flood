Core = {}

project "Core"

	kind "StaticLib"
	location (action)
	objdir (action .. "/obj")
	targetdir (action .. "/lib")
	
	flags { common_flags }

	configuration "vs*"
		pchheader "Core/API.h"
		pchsource "../src/Core/Core.cpp"
		
	configuration "*"
	
	files {
		"Core.lua",
		"../inc/Core/**.h",
		"../src/Core/**.cpp",
		"../inc/Math/**.h",
		"../src/Math/**.cpp",
		"../inc/Network/**.h",
		"../src/Network/**.cpp",
		"../dep/FastLZ/*"
	}
	
	vpaths {
		[""] = { "**src/Core*", "**inc/Core*" },
		["Archive"] = "**/Archive*",
		["Compression"] = "**/fastlz*",
		["Concurrency"] = "**/Concurren*",
		["Cryptography"] = "**/Cipher*",
		["Debug"] = "**/StackWalker*",
		["Events"] = { "**/Delegate*", "**/Event*" },
		["Extensions"] = "**/Plugin*",
		["Libraries"] = "**/DynamicLibrary*",
		["Logs"] = "**/Log*",
		["Math"] = "**/Math/**",
		["Memory"] = { "**/Handle*", "**/Memory*", "**/Pointers*", "**/References*" },
		["Network"] = { "**/Network*" },
		["Reflection"] = { "**/ClassWatcher*", "**/Object*", "**/Reflection*" },
		["Serialization"] = "**/Serialization*",
		["Serialization/Binary"] = "**/BinarySerializer*",
		["Serialization/JSON"] = "**/JsonSerializer*",
		["Streams"] = "**/Stream*",
		["Streams/File"] = "**/FileStream*",
		["Streams/Memory"] = "**/MemoryStream*",
		["Streams/Web"] = "**/WebStream*",
		["Streams/Watcher"] = "**/FileWatcher*",
		["Utilities"] = "**/Utilities*",
		["Utilities/String"] = { "**/String*", "**/Pattern*" },
		["Utilities/Profiler"] = "**/Profiler*",
		["Utilities/Timer"] = "**/Timer*",
		["Utilities/Telemetry"] = "**/Telemetry*",
	}
	
	includedirs {
		"../inc/",
		"../dep/vld/include",
		"../dep/jansson/include",
		"../dep/zziplib/include",
		"../dep/FastLZ",
		"../dep/mongoose",
		"../dep/enet/include",
		"../dep/curl/include",
		"../dep/misc/include",
	}

	Core.libdirs = {
		"../dep/vld/lib/Win32",
		"../dep/curl/lib",
		"../dep/jansson/lib",
		"../dep/zziplib/lib",
		"../dep/zlib/lib/vc10",
		"../dep/enet/lib",
	}
	
	links {
		"jansson_d",
		"zziplib_d",
		"zlibd",
		"enetd",
		"vld",
		"libcurld",
	}

	configuration "windows"
		links { "ws2_32" }
		files { "../src/Win32/Concurrency*.cpp" }