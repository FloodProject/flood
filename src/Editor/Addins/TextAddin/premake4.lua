EditorClient = {}
EditorClient.name = "TextAddin"

project "TextAddin"

    SetupAddin("TextAddin")

    kind "SharedLib"
    language "C#"
    
    location "."
    
    files
    {
        "premake4.lua",
        "**.cs",
        "**.addin.xml",
        path.join("icons","*.png")
    }
    
    links
    {
        "System",
        "System.Drawing",
        "EngineBindings",
        "EngineManaged",
        "Editor.Client",
        "Mono.Addins"
    }
    
    linksnotcopied
    {
        "EngineBindings",
        "EngineManaged",
        "Editor.Client",
        "Mono.Addins"
    }