GUI = {}
GUI.name = "GUI"

project "GUI"

	kind "SharedLib"
	language "C#"
	
	SetupEngineWeaver("GUI.dll")
	
	location "."
	
	files
	{
		"GUI.lua",
		"**.cs",
	}
	
	links { 
		"System", 
		"System.Core",
		"System.Drawing",
		"System.Windows.Forms",
		"EngineManaged"
	}