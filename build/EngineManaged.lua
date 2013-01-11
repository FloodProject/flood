EngineManaged = {}
EngineManaged.name = "EngineManaged"

project "EngineManaged"

	kind "SharedLib"
	language "C++"
	flags { "Managed" }
	
	SetupNativeProjects()
	
	files
	{
		"EngineManaged.lua",
		"../src/EngineManaged/**.cpp",
	}

	vpaths
	{
		["*"] = { "../src/EngineManaged/" },
	}		
	
	includedirs
	{
		"../inc/"
	}
