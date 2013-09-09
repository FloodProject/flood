project "RPCGen"

    kind "ConsoleApp"
    language "C#"
	uuid "42DB45CD-6DF9-4E3A-930F-3B8A82ADB915"
    
    location "."
    
    files
    {
        "**.cs",
        "RPCGen.lua",
        path.join(srcdir, "EngineManaged/RPC/Serialization/Type.cs")
    }
    
    links {
        "System",
        "Weaver",
    }
