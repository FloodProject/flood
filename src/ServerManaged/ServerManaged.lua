ServerManaged = {}
ServerManaged.name = "ServerManaged"

project "ServerManaged"

	kind "SharedLib"
	language "C#"
	
	--flags { common_flags }
	
	location "."
	
	files
	{
		"ServerManaged.lua",
		"**.cs",
	}

	defines
	{
		--"RAVENDB"
	}
	
	links
	{
		"System", 
		"EngineBindings",
		"EngineManaged"
	}