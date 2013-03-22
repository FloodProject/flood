EditorClient = {}
EditorClient.name = "TextAddin"

project "TextAddin"

    SetupAddin()

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
		"EngineManaged",
		"Editor.Client",
        "Mono.Addins"
	}
    
    linksnotcopied
    {
        "EngineManaged",
		"Editor.Client",
        "Mono.Addins"
    }