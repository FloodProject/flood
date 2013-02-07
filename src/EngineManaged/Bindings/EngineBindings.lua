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

	--vpaths
	--{
	--	["*"] = { "../src/EngineManaged/" },
	--}

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
