project "RPCGen"

    kind "ConsoleApp"
    language "C#"
    
    location "."
    
    files
    {
        "**.cs",
        "RPCGen.lua",
        path.join(srcdir, "EngineManaged/RPC/Serialization/Type.cs")
    }
    
    links {
        "System",
        "EngineWeaver",
    }
