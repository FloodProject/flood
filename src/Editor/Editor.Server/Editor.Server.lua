project "Editor.Server"

	SetupManagedProject()
	kind "SharedLib"
	uuid "F92E57A0-9F24-4B42-ADAE-21417A6F74D1"
	
	files
	{
		"Editor.Server.lua",
		"**.cs",
	}
	
	links
	{
		"System",
		"EngineBindings",
		"EngineManaged",
		"Editor.Shared",
	}