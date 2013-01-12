EditorShared = {}
EditorShared.name = "Editor.Shared"

project "Editor.Shared"

	kind "SharedLib"
	language "C#"
	
	dependson { "RPCGen" }

	postbuildcommands
	{ 
		"\"" .. path.join(libdir,"RPCGen.exe") .. "\" -debug -o\"" .. path.join(gendir,EditorShared.name) .. "\" -msbuild=\"".. path.getabsolute(path.join(".", EditorShared.name .. ".csproj")) .."\" \"" .. path.join(libdir,"Editor.Shared.dll") .. "\""
	}
	
	location "."
	
	files
	{
		"Editor.Shared.lua",
		"**.cs",
	}
	
	links {
		"System", 
		"EngineManaged"
		--"FloodManaged"
	}