GUI = {}
GUI.name = "GUI"

project "GUI"

	kind "SharedLib"
	language "C#"
	
	--flags { common_flags }
	
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
	}