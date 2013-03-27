EngineBindings = {}
EngineBindings.name = "EngineBindings"

project "EngineBindings"

	kind "SharedLib"
	language "C++"
	flags { "Managed" }
	
	SetupNativeProjects()

	defines
	{
		Core.defines,
		Resources.defines,
		Graphics.defines,
		Engine.defines,
	}

	files
	{
		"EngineBindings.lua",
		"**.h",
		"**.cpp",
	}

	links
	{
		"Core",
		"Resources",
		"Engine",
		"Graphics"
	}
	
	includedirs
	{
		"../../../inc/"
	}
