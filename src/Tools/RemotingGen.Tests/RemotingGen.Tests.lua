
project "RemotingGen.Tests"

    SetupManagedProject()
    kind "SharedLib"
	uuid "E4FC8997-E7B9-4F88-B741-016B6092197C"
    
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

    SetupManagedProject()
    kind "SharedLib"
	uuid "F8CDF6F2-1B2A-4540-B5C3-5ECED4937C2B"

	SetupRemotingGen("RemotingGen.Tests.Services",".dll")
    
    files { "Services/**.cs" }
    
    links
    {
        "System",
        "EngineManaged",
        "EngineBindings",
        path.join(depsdir,"NUnit","nunit.framework"),
    }
