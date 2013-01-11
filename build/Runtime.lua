Runtime = {}
Runtime.name = "Runtime"
Runtime.defines = {}
Runtime.shared = true

Mono = {}
Mono.links = { "Mswsock", "ws2_32", "psapi", "version", "winmm" }
Mono.shared = true

project "Runtime"

	if Runtime.shared then
		kind "SharedLib"
		table.insert(Runtime.defines, "API_RUNTIME_DLL")
		defines { "API_RUNTIME_DLL_EXPORT" }
	else
		kind "StaticLib"
	end
	
	SetupNativeProjects()

	builddeps { Core.name, Resources.name }
	
	defines { Runtime.defines }

	files
	{
		"Runtime.lua",
		"../src/Runtime/**.h",
		"../src/Runtime/**.cpp",
		"../src/Runtime/**.rc",
	}
	
	vpaths
	{
		["*"] = { "../src/Runtime/", "../inc/Runtime/" },
	}

	includedirs
	{
		"../inc/",
		"../src/",
		"../deps/Mono/eglib/src/"
	}
	
	Runtime.libdirs = 
	{
		Core.libdirs,
		Resources.libdirs,
	}

	libdirs { Runtime.libdirs }

	Runtime.links = 
	{
		Core.name, Core.links,
		Resources.name, Resources.links,
	}

	links { Runtime.links }

	Runtime.deps =
	{
		"AnyOption",
		"Mono"
	}

	deps(Runtime.deps)

	configuration "windows"
		if not Mono.shared then
			links { Mono.links }
		end

	configuration {}