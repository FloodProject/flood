
project "EngineBindings.Tests"

	SetupManagedProject()
	kind "SharedLib"
	uuid "3C32D4AB-C63A-4CCD-B460-CFBD9F51008B"
	
	files
	{
		"EngineBindings.Tests.lua",
		"**.cs",
	}
	
	links
	{
		"System",
		"EngineBindings",
		path.join(depsdir,"NUnit","nunit.framework"),
		path.join(depsdir,"NSubstitute","NSubstitute")
	}