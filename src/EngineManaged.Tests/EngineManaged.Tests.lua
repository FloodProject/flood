
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
		path.join(depsdir,"NUnit","nunit.framework"),
		path.join(depsdir,"NSubstitute","NSubstitute")
	}