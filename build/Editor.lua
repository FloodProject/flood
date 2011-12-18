project "Editor"

	kind "ConsoleApp"
	targetdir "../bin/Editor"
	debugdir "../bin"

	builddeps { "Core", "Resources", "Engine", "Pipeline" }
	
	editor_flags = common_flags
	table.remove(editor_flags, #editor_flags)
	table.remove(editor_flags, #editor_flags)
	
	flags { editor_flags }

	pchheader "Editor/API.h"
	pchsource "../src/Editor/Editor.cpp"
	
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
		"../dep/wx/include",
		"../dep/vld/include",
	}
	
	libdirs
	{
		Core.libdirs,
		Resources.libdirs,
		Engine.libdirs,
		Pipeline.libdirs,
		"../dep/wx/lib/vc_dll",
	}
	
	links { "Core", "Resources", "Graphics", "Engine", "Pipeline" }
	
	configuration "windows"
		defines { "__WXMSW__" }
		includedirs { "../dep/wx/include/msvc" }
		links { "wxmsw29ud","wxmsw29ud_gl" }

	configuration "vs*"
		defines { "_CRT_SECURE_NO_WARNINGS" }

	configuration "*"
		defines { '_LIB', 'WXUSINGDLL', 'WXMONOLITHIC' }
