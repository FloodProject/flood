
project "EngineBindings.CSharp"

	SetupManagedProject()
	kind "SharedLib"
	flags { "Managed", "Unsafe" }
	
	files
	{
		"EngineBindings.CSharp.lua",
		"**.cs",
	}
