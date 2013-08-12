Runtime = {}
Runtime.name = "Runtime"
Runtime.defines = {}
Runtime.shared = true

Mono = {}
Mono.links = { "Mswsock", "ws2_32", "psapi", "version", "winmm" }
Mono.shared = true

project "Runtime"

	uuid "CE017688-F643-4A58-9F87-DE9FA184700E"

	if Runtime.shared then
		kind "SharedLib"
		table.insert(Runtime.defines, "API_RUNTIME_DLL")
		defines { "API_RUNTIME_DLL_EXPORT" }
	else
		kind "StaticLib"
	end
	
	SetupNativeProjects()

	defines { Runtime.defines }

	files
	{
		"Runtime.lua",
		"**.h",
		"**.cpp",
		"**.rc",
	}
	
	vpaths
	{
		["*"] = { ".", path.join(incdir,"Runtime") },
	}

	includedirs
	{
		incdir,
		srcdir,
		path.join(depsdir,"/Mono/eglib/src/");
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