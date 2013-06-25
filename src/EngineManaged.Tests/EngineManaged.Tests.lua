
project "EngineManaged.Tests"

	kind "SharedLib"
	language "C#"
	
	location "."
	
	files
	{
		"EngineManaged.Tests.lua",
		"**.cs",
	}
	
	links {
		"System",
		"EngineManaged",
		"EngineBindings",
		path.join(depsdir,"NUnit","nunit.framework"),
		path.join(depsdir,"NSubstitute","NSubstitute")
	}