
project "Editor.Client"

	kind "SharedLib"
	language "C#"
	uuid "30F908D4-BC8D-429F-830B-EC3561467C82"
	
	location "."

	SetupRemotingGen("Editor.Client", ".dll")
	
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
	}