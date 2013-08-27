EditorClient = {}
EditorClient.name = "Editor.Client"

project "Editor.Client"

	kind "SharedLib"
	language "C#"
	location "."
	flags { "Unsafe" }
	uuid "BDDF5511-B91A-48B1-8E88-23797D771406"
	
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
		"ServerManaged"
	}