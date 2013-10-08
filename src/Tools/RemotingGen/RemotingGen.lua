project "RemotingGen"

    kind "ConsoleApp"
    language "C#"
    uuid "42DB45CD-6DF9-4E3A-930F-3B8A82ADB915"
    
    location "."
    
    files
    {
        "**.cs",
        "RemotingGen.lua",
        path.join(srcdir, "EngineManaged/Remoting/Serialization/Type.cs")
    }
    
    links {
        "System",
        "Weaver",
        "Mono.Cecil"
    }
