project "Editor.Shared"

	SetupManagedProject()
	kind "SharedLib"
	uuid "B0811D44-B7E9-43A0-BB93-6B05E4F91385"
	
	files
	{
		"Editor.Shared.lua",
		"**.cs",
	}
	
	links
	{
		"System", 
		"EngineManaged"
	}