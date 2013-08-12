GameRuntime = {}
GameRuntime.name = "GameRuntime"

project "GameRuntime"

	uuid "92D7D852-240F-44EF-BAB6-1B59A227266F"

	kind "ConsoleApp"

	SetupNativeProjects()
	
	defines
	{
		Engine.defines,	-- includes Graphics.defines
		Runtime.defines,
		"SFML_WINDOW_EXPORTS",
	}

	files
	{
		"GameRuntime.lua",
		"**.h",
		"**.cpp",
		"../Platforms/SFML/*.h",
		"../Platforms/SFML/*.cpp",
	}

	vpaths
	{
		["*"] = { ".", path.join(incdir,"GameRuntime"), srcdir },
	}

	includedirs
	{
		incdir,
		srcdir,
		path.join(depsdir,"SFML/include/"),
	}
	
	libdirs
	{
		Engine.libdirs,
		Graphics.libsdirs,
		Runtime.libdirs,
		Pipeline.libdirs,
	}

	links
	{
		Engine.name, Engine.links,
		Graphics.name, Graphics.links,
		Runtime.name, Runtime.links,
		Pipeline.name, Pipeline.links

	}

	GameRuntime.deps =
	{
		Engine.deps, -- Engine.deps contains Graphics.deps
		Runtime.deps,
		Pipeline.deps,
		"SFML",
	}

	deps { GameRuntime.deps }