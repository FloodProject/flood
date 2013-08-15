
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
	excludes { "Interfaces/*" }
    
    links
    {
        "System",
        "RPCGen",
        "EngineManaged",
		"RPCGen.Tests.Interfaces",
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
    
    files { "Services/**.cs" }
    
    links
    {
        "System",
        "EngineManaged",
        "EngineBindings",
		"RPCGen.Tests.Interfaces",
        path.join(depsdir,"NUnit","nunit.framework"),
    }

project "RPCGen.Tests.Interfaces"

    kind "SharedLib"
    language "C#"
	uuid "3D99B14D-803E-4EC5-81E1-98E9C2057321"
    
    location "Interfaces/"
    
    files { "Interfaces/**.cs" }