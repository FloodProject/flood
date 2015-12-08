
project "Weaver"

    SetupManagedProject()
    kind "SharedLib"
	uuid "B9BAAEB3-DA30-4E96-B1F2-4548385B9D06"
    
    files
    {
        "Weaver.lua",
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