EditorClient = {}
EditorClient.name = "Editor.Client"

project "Editor.Client"

	kind "SharedLib"
	language "C#"
	
	location "."
	
	files
	{
		"Editor.Client.lua",
		"**.cs",
	}
	
	links
	{
		"System",
		"System.ComponentModel", 
		"System.ComponentModel.Composition",
		"Editor.Shared",
		"Editor.Server",
		"GUI" 
	}