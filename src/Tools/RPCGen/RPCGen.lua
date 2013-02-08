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
		path.join(depsdir,"NUnit","nunit.framework"),
		path.join(depsdir,"NSubstitute","NSubstitute")
	}