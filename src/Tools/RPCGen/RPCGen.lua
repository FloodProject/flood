project "RPCGen"

    kind "ConsoleApp"
    language "C#"
    
    location "."
    
    files
    {
        "**.cs",
        "RPCGen.lua",
        path.join(srcdir, "EngineManaged/RPC/Protocol/Type.cs")
    }
    
    links {
        "System",
        "EngineWeaver",
    }
