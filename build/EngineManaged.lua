project "EngineManaged"

	kind "SharedLib"
	language "C++"
	flags { "Managed" }
	
	files
	{
		"EngineManaged.lua",
		"../src/EngineManaged/*.cpp",
	}

	vpaths
	{
		["*"] = { "../src/EngineManaged/", "../inc/EngineManaged/" },
	}		
	
	includedirs
	{
		"../inc/"
	}
