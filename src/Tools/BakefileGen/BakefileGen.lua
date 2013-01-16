BakefileGen = {}
BakefileGen.name = "BakefileGen"

project "BakefileGen"

	kind "ConsoleApp"
	language "C#"
	
	location "."
	
	files
	{
		"BakefileGen.lua",
		"**.cs",
	}
	
	links
	{
		"System",
		"System.Xml",
		"System.Xml.Linq"
	}