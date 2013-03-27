EngineManaged = {}
EngineManaged.name = "EngineManaged"

project "EngineManaged"

	kind "SharedLib"
	language "C#"
	
	location "."
	
	files
	{
		"EngineManaged.lua",
		"**.cs",
	}
	
	links {
		"EngineBindings",
		"System",
		"System.Web",
		"System.Windows.Forms"
	}