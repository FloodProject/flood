project "Mono.Addins"
    
    SetupManagedDependencyProject()

    kind "SharedLib"
    language "C#"
    
    files 
    { 
        "Mono.Addins/**.cs" 
    }
    
    links
    {
        "System",
        "System.Core",
        "System.Xml",
    }