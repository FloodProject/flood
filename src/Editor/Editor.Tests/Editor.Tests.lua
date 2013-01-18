EditorTests = {}
EditorTests.name = "Editor.Tests"

project "Editor.Tests"

	kind "SharedLib"
	language "C#"
	
	location "."
	
	files
	{
		"Editor.Tests.lua",
		"**.cs",
	}
	
	links
	{
		"System",
		"System.ComponentModel", 
		"System.ComponentModel.Composition",
		"EngineManaged",
		"Editor.Client",
		"Editor.Shared",
		"Editor.Server",
		"ServerManaged",
		"GUI",
		path.join(depsdir,"NUnit","nunit.framework"),
		path.join(depsdir,"NSubstitute","NSubstitute")
	}