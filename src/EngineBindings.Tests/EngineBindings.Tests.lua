
project "EngineBindings.Tests"

	kind "SharedLib"
	language "C#"
	
	location "."
	
	files
	{
		"EngineBindings.Tests.lua",
		"**.cs",
	}
	
	links {
		"System",
		"EngineBindings",
		path.join(depsdir,"NUnit","nunit.framework"),
		path.join(depsdir,"NSubstitute","NSubstitute")
	}