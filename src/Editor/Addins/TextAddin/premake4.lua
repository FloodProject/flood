
project "TextAddin"

    SetupAddin("TextAddin")

    kind "SharedLib"
    language "C#"
    
    location "."
    
    EmbedFiles {
        "**.xshd",
        "**.xsd"
    }

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
        "System.XML",
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