
project "EngineWeaver"

    kind "SharedLib"
    language "C#"
	uuid "B9BAAEB3-DA30-4E96-B1F2-4548385B9D06"
    
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