EngineBindings = {}
EngineBindings.name = "EngineBindings"

project "EngineBindings"

	kind "SharedLib"
	language "C++"
	flags { "Managed" }
	
	SetupNativeProjects()
	
	local c = configuration "Debug"
		targetsuffix ""
	configuration(c)

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
