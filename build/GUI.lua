GUI = {}
GUI.name = "GUI"

project "GUI"

	kind "SharedLib"
	language "C#"
	
	--flags { common_flags }
	
	location "../src/GUI"
	
	files
	{
		"GUI.lua",
		"../src/GUI/**.cs",
	}
	
	links { 
		"System", 
		"System.Core",
		"System.Drawing",
		"System.Windows.Forms",
	}