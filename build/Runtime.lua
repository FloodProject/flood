Runtime = {}
Runtime.name = "Runtime"

Mono = {}
Mono.links = { "Mswsock", "ws2_32", "psapi", "version", "winmm" }

project "Runtime"

	kind "ConsoleApp"
	builddeps { "Core", "Resources" }
	targetdir "../bin/"

	--pchheader "Runtime/API.h"
	--pchsource "../src/Runtime/Runtime.cpp"
	
	files
	{
		"Runtime.lua",
		"../src/Runtime/**.h",
		"../src/Runtime/**.cpp",
		"../src/Runtime/**.rc",
	}
	
	vpaths
	{
		[""] = { "../src/Runtime/", "../inc/Runtime/" },
	}

	includedirs
	{
		"../inc/",
		"../src/",
		"../deps/Mono/eglib/src/"
	}
	
	libdirs
	{
		Core.libdirs,
		Resources.libdirs,
	}

	links
	{
		Core.name, Core.links,
		Resources.name, Resources.links,
	}

	Runtime.deps =
	{
		"AnyOption",
		"Mono"
	}

	deps(Runtime.deps)

	configuration "windows"
		links { Mono.links }

	configuration {}