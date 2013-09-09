project "Editor"

	kind "ConsoleApp"
	language "C#"
	location "."
	flags { "Unsafe" }
	uuid "CA9C6CED-8FC5-4A59-AFB1-783BF1DD5A71"
	
	dependson { "Editor.Native" }

	debugdir(bindir)

	files
	{
		"Editor.lua",
		"**.cs",
	}
	
	links
	{
		"EngineManaged",
		"EngineBindings",
		"Editor.Client",
		"Editor.Shared",
		"Editor.Server",
	}