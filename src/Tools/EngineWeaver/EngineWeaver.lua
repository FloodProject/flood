
project "EngineWeaver"

    kind "SharedLib"
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
        "ICSharpCode.Decompiler",
        "ICSharpCode.NRefactory",
        "ICSharpCode.NRefactory.CSharp"
    }