project "Editor.Tests"

	SetupManagedProject()
	kind "SharedLib"
	uuid "8AA944C5-4C06-4EE9-93B7-44720E5630B4"
	
	files
	{
		"Editor.Tests.lua",
		"**.cs",
	}
	
	links
	{
		"Editor.Client",
		"Editor.Shared",
		"Editor.Server",
		path.join(depsdir,"NUnit","nunit.framework"),
		path.join(depsdir,"NSubstitute","NSubstitute")
	}