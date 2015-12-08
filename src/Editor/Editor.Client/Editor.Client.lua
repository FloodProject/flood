
project "Editor.Client"

	SetupManagedProject()
	kind "SharedLib"
	uuid "30F908D4-BC8D-429F-830B-EC3561467C82"

	SetupModule("Editor.Client")
	
	files
	{
		"Editor.Client.lua",
		"**.cs",
	}
	
	links
	{
		"System",
		"EngineBindings",
		"EngineManaged",
		"Editor.Server"
	}