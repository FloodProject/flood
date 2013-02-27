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
		"**.addin.xml"
	}
	
	links
	{
		"System",
		"System.Drawing", 
		"EngineManaged",
		"EngineBindings",
		"Editor.Shared",
		"Editor.Server",
		"ServerManaged",
        "Mono.Addins"
	}