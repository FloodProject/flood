EditorManaged = {}
EditorManaged.name = "EditorManaged"

project "EditorManaged"

	kind "SharedLib"
	language "C#"
	
	--flags { common_flags }
	
	location "../src/Editor/Managed/"
	
	files
	{
		"EditorManaged.lua",
		"../src/Editor/Managed/**.cs",
	}
	
	--links { "System", "EngineManaged" }
	links {
		"System", 
		"System.ComponentModel", 
		"System.ComponentModel.Composition",
		"GUI" 
	}