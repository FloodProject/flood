project "RemotingGen"

    SetupManagedProject()
    kind "ConsoleApp"
    uuid "42DB45CD-6DF9-4E3A-930F-3B8A82ADB915"
    
    files
    {
        "**.cs",
        "RemotingGen.lua",
        path.join(srcdir, "EngineManaged/Serialization/DataType.cs")
    }
    
    links
    {
        "System",
        "Weaver",
        "Mono.Cecil"
    }
