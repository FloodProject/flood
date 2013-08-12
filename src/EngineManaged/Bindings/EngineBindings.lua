EngineBindings = {}
EngineBindings.name = "EngineBindings"

project "EngineBindings"

	kind "SharedLib"
	language "C++"
	uuid "F8A701FB-361E-44F6-838A-9B5790F7801F"

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
