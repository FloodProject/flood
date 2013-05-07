EditorRuntime = {}
EditorRuntime.defines = {}

function SetupWxWidgets()
	wxWidgets = {}
	
	wxWidgets.includedirs =
	{ 
		path.join(depsdir,"wxWidgets/include"),
	}

	wxWidgets.libdirs = { }	
	wxWidgets.links = { }
	wxWidgets.defines = { '_LIB', 'WXUSINGDLL', 'WXMONOLITHIC' }

	defines { wxWidgets.defines }
    
    SetupAddinsAsDependencies()

	local c = configuration "windows"
		defines { "__WXMSW__" }

	configuration (c)
end

project "EditorRuntime"

	debugdir(bindir)
	
	SetupNativeProjects()

	dependson { EngineManaged.name, EditorClient.name }
	
	defines
	{
		Core.defines,
		Resources.defines,
		Graphics.defines,
		Engine.defines,
		Pipeline.defines
	}

	editor_flags = common_flags
	table.remove(editor_flags, #editor_flags)
	
	flags { editor_flags }

	pchheader "Editor/API.h"
	pchsource "Editor.cpp"
	
	configuration "Debug"
		kind "ConsoleApp"
	
	configuration "Release"
		kind "WindowedApp"
		flags { "WinMain" }
		
	configuration "*"	
	
	files
	{
		"EditorRuntime.lua",
		path.join(incdir,"Editor/**.h"),
		"**.h",
		"**.cpp",
		"**.rc",
	}

	configuration "Managed/GwenRenderer.cpp"
		usingdirs { libdir }
		flags { "Managed", "NoMinimalRebuild", "NoRuntimeChecks", "NoPCH" }
	configuration "*"

	vpaths
	{
		["*"] = { ".", path.join(incdir,"Editor") },
	}

	EditorRuntime.deps = { "wxWidgets" }
	if config.SCRIPTING_MONO then
		table.insert(EditorRuntime.deps,"Mono")
	end
	
	SetupWxWidgets()

	includedirs
	{
		incdir,
		srcdir,
		path.join(incdir,"Editor"),
		path.join(srcdir,"EditorRuntime"),
		path.join(srcdir,"EditorRuntime","Widgets"),
		wxWidgets.includedirs
	}

	libdirs
	{
		libdir,
		Core.libdirs,
		Resources.libdirs,
		Engine.libdirs,
		Pipeline.libdirs,
		wxWidgets.libdirs,
	}
	
	links
	{
		Engine.name,
		Engine.links,
		Pipeline.name,
		Pipeline.links,
		wxWidgets.links
	}

	deps { EditorRuntime.deps }
		
	configuration "vs*"
		defines { "_CRT_SECURE_NO_WARNINGS" }
	
	if config.SCRIPTING_MONO then
	  configuration "windows"
		  links { Mono.links }
	end
