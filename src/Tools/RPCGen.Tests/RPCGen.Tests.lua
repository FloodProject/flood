
project "RPCGen.Tests"

    kind "SharedLib"
    language "C#"
	uuid "E4FC8997-E7B9-4F88-B741-016B6092197C"
    
    location "."
    
    files
    {
        "RPCGen.Tests.lua",
        "**.cs",
    }
    
    excludes { "Services/*" }
    
    links
    {
        "System",
        "RPCGen",
        "EngineManaged",
        path.join(depsdir,"NUnit","nunit.framework"),
    }

    dependson 
    {
        "RPCGen.Tests.Services"
    }

project "RPCGen.Tests.Services"

    kind "SharedLib"
    language "C#"
	uuid "F8CDF6F2-1B2A-4540-B5C3-5ECED4937C2B"
    
    location "Services/"

	SetupRPCGen("RPCGen.Tests.Services",".dll")
    
    files { "Services/**.cs" }
    
    links
    {
        "System",
        "EngineManaged",
        "EngineBindings",
        path.join(depsdir,"NUnit","nunit.framework"),
    }
