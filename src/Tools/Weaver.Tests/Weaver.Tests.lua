
project "Weaver.Tests"

    kind "SharedLib"
    language "C#"
	uuid "9DE4213F-3B92-4674-B92B-64E84BD9B54A"
    
    location "."
    
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