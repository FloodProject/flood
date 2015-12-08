
project "EngineManaged.Tests"

	SetupManagedProject()
	kind "SharedLib"
	uuid "000E000F-F309-4EAA-AC69-0D71ED1015ED"
	
	SetupRemotingGen("EngineManaged.Tests",".dll")

	files
	{
		"EngineManaged.Tests.lua",
		"**.cs",
	}
	
	links {
		"System",
		"System.Data",
		"System.Xml",
		"EngineManaged",
		"EngineBindings",
		path.join(depsdir,"NUnit","nunit.framework"),
		path.join(depsdir,"NSubstitute","NSubstitute")
	}