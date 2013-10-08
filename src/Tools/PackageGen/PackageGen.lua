project "PackageGen"

    kind "ConsoleApp"
    language "C#"
    uuid "4236C195-6366-4D09-B2BC-7C1C80B165D3"

    location "."
    
    files
    {
        "**.cs",
        "PackageGen.lua"
    }
    
    links {
        "System",
        "EngineManaged",
        "RemotingGen",
        path.join(depsdir,"DotNetZip","Ionic.Zip")
    }
