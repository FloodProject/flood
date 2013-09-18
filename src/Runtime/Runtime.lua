project "Runtime"

	kind "ConsoleApp"
	language "C#"
	location "."

	uuid "CA9C6CED-8FC5-4A59-AFB1-783BF1DD5A71"

	files
	{
		"Runtime.lua",
		"**.cs",
	}
	
	links
	{
		"EngineManaged",
		"EngineBindings",
	}

	SetupPackagesAsDependencies()