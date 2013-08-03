EditorShared = {}
EditorShared.name = "Editor.Shared"

project "Editor.Shared"

	kind "SharedLib"
	language "C#"
	
	location "."
	
	files
	{
		"Editor.Shared.lua",
		"**.cs",
	}
	
	links
	{
		"System", 
		"EngineManaged"
	}