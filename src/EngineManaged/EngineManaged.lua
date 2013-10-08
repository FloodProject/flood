EngineManaged = {}
EngineManaged.name = "EngineManaged"

project "EngineManaged"

	kind "SharedLib"
	language "C#"
	uuid "4A3F50C0-E648-4F10-826C-8D5D7A9B65A0"

	flags { "Unsafe" }
	location "."

	SetupRemotingGen("EngineManaged", ".dll")

	files
	{
		"EngineManaged.lua",
		"**.cs"
	}

	excludes { "**Bindings.CSharp**" }
	
	links
	{
		"EngineBindings",
		"System",
		path.join(depsdir,"DotNetZip","Ionic.Zip")
	}