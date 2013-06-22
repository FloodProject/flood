
project "RPCGen.Tests"

    kind "SharedLib"
    language "C#"
    
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
    
    location "Services/"
    
    files { "Services/**.cs" }
    
    links
    {
        "System",
        "EngineManaged",
        path.join(depsdir,"NUnit","nunit.framework"),
    }