
project "Weaver.Tests"

    SetupManagedProject()
    kind "SharedLib"
	uuid "9DE4213F-3B92-4674-B92B-64E84BD9B54A"
    
    files
    {
        "Weaver.Tests.lua",
        "**.cs",
    }
    
    links
    {
        "System",
        "EngineManaged",
		"Weaver",
        "Mono.Cecil",
		path.join(depsdir,"NUnit","nunit.framework")
    }