
project "EngineManaged.Tests"

	kind "SharedLib"
	language "C#"
	uuid "000E000F-F309-4EAA-AC69-0D71ED1015ED"
	
	location "."
	
	SetupRemotingGen("EngineManaged.Tests",".dll")

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