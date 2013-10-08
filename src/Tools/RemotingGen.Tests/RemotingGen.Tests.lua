
project "RemotingGen.Tests"

    kind "SharedLib"
    language "C#"
	uuid "E4FC8997-E7B9-4F88-B741-016B6092197C"
    
    location "."
    
    files
    {
        "RemotingGen.Tests.lua",
        "**.cs",
    }
    
    excludes { "Services/*" }
    
    links
    {
        "System",
        "RemotingGen",
        "EngineManaged",
        path.join(depsdir,"NUnit","nunit.framework"),
    }

    dependson 
    {
        "RemotingGen.Tests.Services"
    }

project "RemotingGen.Tests.Services"

    kind "SharedLib"
    language "C#"
	uuid "F8CDF6F2-1B2A-4540-B5C3-5ECED4937C2B"
    
    location "Services/"

	SetupRemotingGen("RemotingGen.Tests.Services",".dll")
    
    files { "Services/**.cs" }
    
    links
    {
        "System",
        "EngineManaged",
        "EngineBindings",
        path.join(depsdir,"NUnit","nunit.framework"),
    }
