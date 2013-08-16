
project "EngineBindings.CSharp"

	kind "SharedLib"
	language "C#"
	flags { "Managed", "Unsafe" }
	
	files
	{
		"EngineBindings.CSharp.lua",
		"**.cs",
	}
