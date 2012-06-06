project "EditorManaged"

	kind "SharedLib"
	language "C#"
	
	targetdir "../bin"
	debugdir "../bin"
	
	flags { common_flags }
	
	files
	{
		"EditorManaged.lua",
		"../interface/Bindings/Editor/**.cs",
		"../src/Editor/Managed/**.cs",
	}
	
	--vpaths
	--{
		--[""] = { "../../src/Editor/Managed/" },
	--}
	
	links { "System", "EngineManaged" }