EditorServer = {}
EditorServer.name = "Editor.Server"

project "Editor.Server"

	kind "SharedLib"
	language "C#"
	uuid "F92E57A0-9F24-4B42-ADAE-21417A6F74D1"
	
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