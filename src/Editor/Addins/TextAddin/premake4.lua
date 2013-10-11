
project "TextAddin"

    kind "SharedLib"
    language "C#"
    
    location "."
    
    EmbedFiles {
        "**.xshd",
        "**.xsd"
    }

    SetupModule("TextAddin")

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
        "Editor.Client"
    }
    
    linksnotcopied
    {
        "EngineBindings",
        "EngineManaged",
        "Editor.Client"
    }