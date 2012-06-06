project "Runtime"

	kind "WindowedApp"
	targetdir "../bin/Runtime"
	debugdir "../bin"

	builddeps { "Core", "Resources", "Engine", "Pipeline" }
	
	flags { common_flags }

	--pchheader "Engine/API.h"
	--pchsource "../src/Runtime/Runtime.cpp"
	
	files
	{
		"Runtime.lua",
		"../src/Runtime/**.h",
		"../src/Runtime/**.cpp",
		"../src/Runtime/**.rc",
		"../dep/AnyOption/*.cpp",
		"../dep/AnyOption/*.h",
	}
	
	vpaths
	{
		[""] = { "../../src/Runtime/", "../../inc/Runtime/" },
	}

	includedirs
	{
		"../inc/",
		"../src/",
		"../dep/mono/include",
		"../dep/vld/include",
		"../dep/AnyOption"
	}
	
	libdirs
	{
		Core.libdirs,
		Resources.libdirs,
		Engine.libdirs,
		Pipeline.libdirs,
		"../dep/wx/lib/vc_dll",
		"../dep/mono/lib/",
	}
	
	links
	{
		"Core", Core.links,
		"Resources", Resources.links,
		"Graphics", Graphics.links,
		"Engine", Engine.links,
		"Pipeline", Pipeline.links,
		Mono.links,
	}
	
	configuration "windows"
		defines { "__WXMSW__" }
		includedirs { "../dep/wx/include/msvc" }
		links { "wxmsw29ud","wxmsw29ud_gl" }

	configuration "vs*"
		defines { "_CRT_SECURE_NO_WARNINGS" }

	configuration "*"
		defines { '_LIB', 'WXUSINGDLL', 'WXMONOLITHIC' }
