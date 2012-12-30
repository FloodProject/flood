GameRuntime = {}
GameRuntime.name = "GameRuntime"

project "GameRuntime"

	kind "ConsoleApp"
	builddeps { Engine.name, Graphics.name, Runtime.name }
	
	defines
	{
		Engine.defines,	-- includes Graphics.defines
		Runtime.defines,
		"SFML_WINDOW_EXPORTS",
	}

	files
	{
		"GameRuntime.lua",
		"../src/GameRuntime/**.h",
		"../src/GameRuntime/**.cpp",
		"../src/Platforms/SFML/*.h",
		"../src/Platforms/SFML/*.cpp",
	}

	vpaths
	{
		["*"] = { "../src/GameRuntime/", "../inc/GameRuntime/" },
	}

	includedirs
	{
		"../inc/",
		"../src/",
		"../deps/SFML/include/",
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