RPCGen = {}
RPCGen.name = "RPCGen"

project "RPCGen"

	kind "ConsoleApp"
	language "C#"
	
	location "."
	
	files
	{
		"RPCGen.lua",
		"**.cs",
	}
	
	links {
		"System",
		"EngineManaged"
	}