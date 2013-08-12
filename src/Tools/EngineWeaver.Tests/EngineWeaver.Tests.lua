
project "EngineWeaver.Tests"

    kind "SharedLib"
    language "C#"
	uuid "9DE4213F-3B92-4674-B92B-64E84BD9B54A"
    
    location "."
    
    files
    {
        "EngineWeaver.Tests.lua",
        "**.cs",
    }
    
    links
    {
        "System",
        "EngineManaged",
		"EngineWeaver",
        "Mono.Cecil",
		path.join(depsdir,"NUnit","nunit.framework")
    }