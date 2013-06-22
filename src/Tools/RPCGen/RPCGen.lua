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
		"EngineManaged",
		"EngineWeaver",
		path.join(depsdir,"NUnit","nunit.framework"),
		path.join(depsdir,"NSubstitute","NSubstitute")
	}