
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
        "EngineManaged",
		"Mono.Cecil.Pdb",
        "Mono.Cecil",
        "ICSharpCode.Decompiler",
        "ICSharpCode.NRefactory",
        "ICSharpCode.NRefactory.CSharp"
    }