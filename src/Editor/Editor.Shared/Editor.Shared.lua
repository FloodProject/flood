EditorShared = {}
EditorShared.name = "Editor.Shared"

project "Editor.Shared"

	kind "SharedLib"
	language "C#"
	
	prebuildcommands
	{ 
		--"luac -o  "
	}
	
	location "."
	
	files
	{
		"Editor.Shared.lua",
		"**.cs",
	}
	
	links {
		"System", 
		--"FloodManaged"
	}