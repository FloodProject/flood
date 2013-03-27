EngineBindings = {}
EngineBindings.name = "EngineBindings"

project "EngineBindings"

	kind "SharedLib"
	language "C++"
	flags { "Managed" }
	
	SetupNativeProjects()

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
