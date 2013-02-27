EditorClient = {}
EditorClient.name = "Editor.Client"

project "Editor.Client"

	kind "SharedLib"
	language "C#"
	
	location "."
	
    SetupAddinResources()
    
	files
	{
		"Editor.Client.lua",
		"**.cs",
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