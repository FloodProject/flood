
project "EngineWeaver"

    kind "ConsoleApp"
    language "C#"
    
    location "."
    
    files
    {
        "EngineWeaver.lua",
        "**.cs",
    }
    
    links
    {
        "System",
        "Mono.Cecil.Pdb",
        "Mono.Cecil",
        "EngineManaged",
        "ICSharpCode.Decompiler",
        "ICSharpCode.NRefactory",
        "ICSharpCode.NRefactory.CSharp"
    }