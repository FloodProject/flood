EditorClient = {}
EditorClient.name = "Editor.Client"

project "Editor.Client"

	kind "SharedLib"
	language "C#"
	
	prebuildcommands
	{ 
		--"luac -o  "
	}
	
	location "."
	
	files
	{
		"Editor.Client.lua",
		"**.cs",
	}
	
	links {
		"System", 
		"Editor.Shared",
		"System.ComponentModel", 
		"System.ComponentModel.Composition",
		"GUI" 
	}