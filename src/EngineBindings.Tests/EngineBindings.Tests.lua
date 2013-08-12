
project "EngineBindings.Tests"

	kind "SharedLib"
	language "C#"
	uuid "3C32D4AB-C63A-4CCD-B460-CFBD9F51008B"
	
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