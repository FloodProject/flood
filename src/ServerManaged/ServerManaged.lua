ServerManaged = {}
ServerManaged.name = "ServerManaged"

project "ServerManaged"

	kind "SharedLib"
	language "C#"
	uuid "E7CB0022-89E0-4C09-A8AF-F06B4D2B38D3"
	
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