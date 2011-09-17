project "Editor"

	kind "WindowedApp"
	location (action)
	objdir (action .. "/obj")
	targetdir "../bin/Editor"

	builddeps { "Core", "Resources", "Engine", "Pipeline" }
	
	flags { common_flags }

	pchheader "Editor/API.h"
	pchsource "../src/Editor/Editor.cpp"
	
	files {
		"Editor.lua",
		"../inc/Editor/**.h",
		"../src/Editor/**.h",
		"../src/Editor/**.cpp",
	}
	
	vpaths {
		[""] = { "**src/Editor*", "**inc/Editor*" },
		["Document"] = { "**/*Document*" },
		["Events"] = { "**/*Events*" },
		["Input"] = { "**/*EditorInput*" },
		["Plugins"] = { "**/plugins/*" },
		["Preferences"] = { "**/Preferences*" },
		--["Resources"] = { "**/Resources*" },
		["Settings"] = { "**/Settings*" },
		["View"] = { "**/Viewframe*" },
		["Widgets"] = { "**/widgets/*", "**/wx*.h" },
		["Window"] = { "**/RenderControl*", "**/RenderWindow*" },
	}

	includedirs {
		"../src/",
		"../src/Editor",
		"../inc/",
		"../inc/Editor",
		"../dep/wx/include"
	}
	
	libdirs {
		Core.libdirs,
		Resources.libdirs,
		Engine.libdirs,
		Pipeline.libdirs,
		"../dep/wx/lib/vc_dll"
	}
	
	links { "Core", "Resources", "Engine", "Pipeline" }
	
	defines { 'WXUSINGDLL', 'WXMONOLITHIC' }
	
	configuration "windows"
	
		defines { "__WXMSW__" }
		includedirs { "../dep/wx/include/msvc" }
		links { "wxmsw29ud","wxmsw29ud_gl" }

	configuration "vs*"
	
		defines { "_CRT_SECURE_NO_WARNINGS" }