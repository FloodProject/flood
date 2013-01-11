EditorServer = {}
EditorServer.name = "Editor.Server"

project "Editor.Server"

	kind "SharedLib"
	language "C#"
	
	prebuildcommands
	{ 
		--"luac -o  "
	}
	
	location "."
	
	files
	{
		"Editor.Server.lua",
		"**.cs",
	}
	
	links {
		"System", 
		"Editor.Shared",
		--"FloodManaged"
	}