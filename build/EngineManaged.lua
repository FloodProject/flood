project "EngineManaged"

	kind "SharedLib"
	language "C#"
	
	targetdir "../bin"
	debugdir "../bin"
	
	files
	{
		"EngineManaged.lua",
		"../interface/Bindings/Engine/*.cs",
		"../src/Engine/Managed/*.cs",
	}
	
	--vpaths
	--{
		--[""] = { "../../src/Editor/Managed/" },
	--}
	
	links { "System" }