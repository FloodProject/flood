EngineManaged = {}
EngineManaged.name = "EngineManaged"

project "EngineManaged"

	kind "SharedLib"
	language "C++"
	flags { "Managed" }
	
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
