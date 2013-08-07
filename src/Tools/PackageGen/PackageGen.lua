project "PackageGen"

    kind "ConsoleApp"
    language "C#"
    
    location "."
    
    files
    {
        "**.cs",
        "PackageGen.lua"
    }
    
    links {
        "System",
        "EngineManaged",
        "EngineWeaver",
    }
