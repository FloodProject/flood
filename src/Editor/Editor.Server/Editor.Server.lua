EditorServer = {}
EditorServer.name = "Editor.Server"

project "Editor.Server"

	kind "SharedLib"
	language "C#"
	location "."

	files
	{
		"Editor.Server.lua",
		"**.cs",
	}
	
	links
	{
		"System",
		"ServerManaged",
		"EngineBindings",
		"EngineManaged",
		"Editor.Shared",
	}