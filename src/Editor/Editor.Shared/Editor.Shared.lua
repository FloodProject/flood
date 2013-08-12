EditorShared = {}
EditorShared.name = "Editor.Shared"

project "Editor.Shared"

	kind "SharedLib"
	language "C#"
	uuid "B0811D44-B7E9-43A0-BB93-6B05E4F91385"

	location "."

	SetupRPCGen("Editor.Shared", ".dll")
	
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