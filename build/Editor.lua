project "Editor"

	targetdir "../bin/Editor"
	targetname "FlushEditor"
	debugdir "../bin"

	builddeps { "Core", "Resources", "Engine", "Pipeline", "EditorManaged" }
	
	editor_flags = common_flags
	table.remove(editor_flags, #editor_flags)
	table.remove(editor_flags, #editor_flags)
	
	flags { editor_flags }

	pchheader "Editor/API.h"
	pchsource "../src/Editor/Editor.cpp"
	
	configuration "Debug"
		kind "ConsoleApp"
		targetsuffix "_d"
	
	configuration "Release"
		kind "WindowedApp"
		flags { "WinMain" }
		
	configuration "*"	
	
	files
	{
		"Editor.lua",
		"../inc/Editor/**.h",
		"../src/Editor/**.h",
		"../src/Editor/**.cpp",
		"../src/Editor/**.rc",
	}
	
	vpaths
	{
		[""] = { "../../src/Editor/", "../../inc/Editor/" },
		["Document"] = { "Document*" },
		["Events"] = { "*Events*" },
		["Input"] = { "*EditorInput*" },
		["Plugins"] = { "plugins/*" },
		["Preferences"] = { "Preferences*" },
		["Resources"] = { "Resources*" },
		["Settings"] = { "Settings*" },
		["View"] = { "Viewframe*" },
		["Widgets"] = { "widgets/*", "wx*.h" },
		["Window"] = { "RenderControl*", "RenderWindow*" },
	}

	includedirs
	{
		"../inc/",
		"../src/",
		"../inc/Editor",
		"../src/Editor",
		"../src/Editor/Widgets",
		"../dep/wx/include",
		"../dep/mono/include",
		"../dep/vld/include",
	}
	
	libdirs
	{
		Core.libdirs,
		Resources.libdirs,
		Engine.libdirs,
		Pipeline.libdirs,
		"../dep/wx/lib/vc_dll",
		"../dep/mono/lib/",
	}
	
	links
	{
		"Core", Core.links,
		"Resources", Resources.links,
		"Graphics", Graphics.links,
		"Engine", Engine.links,
		"Pipeline", Pipeline.links,
		Mono.links,
		wxWidgets.links
	}

	configuration "Debug"
		links 
		{
			Core.links.Debug,
			Resources.links.Debug,
			Graphics.links.Debug,
			Engine.links.Debug,
			Pipeline.links.Debug
		}
		
	configuration "Release"
		links
		{
			Core.links.Release,
			Resources.links.Release,
			Graphics.links.Release,
			Engine.links.Release,
			Pipeline.links.Release
		}
	
	configuration "windows"
		defines { "__WXMSW__" }
		includedirs { "../dep/wx/include/msvc" }
		
	configuration { "windows", "Debug" }
		links { "wxmsw29ud","wxmsw29ud_gl" }
		
	configuration { "windows", "Release" }
		links { "wxmsw29u","wxmsw29u_gl" }
		
	configuration "vs*"
		defines { "_CRT_SECURE_NO_WARNINGS" }

	configuration "*"
		defines { '_LIB', 'WXUSINGDLL', 'WXMONOLITHIC' }
