workspace "BakefileGen"

	configurations { "Debug" }
	platforms { "x32" }

	project "BakefileGen"

		kind "ConsoleApp"
		
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