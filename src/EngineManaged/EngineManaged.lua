EngineManaged = {}
EngineManaged.name = "EngineManaged"

project "EngineManaged"

	kind "SharedLib"
	language "C#"
	flags { "Unsafe" }
	location "."

	SetupRPCGen("EngineManaged",".dll")
	
	files
	{
		"EngineManaged.lua",
		"**.cs",
	}
	
	links
	{
		"EngineBindings",
		"System",
		"System.Web",
		"System.Windows.Forms"
	}