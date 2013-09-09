function SetupWxWidgets()
	wxWidgets = {}
	
	wxWidgets.includedirs =
	{ 
		path.join(depsdir,"wxWidgets/include"),
	}

	wxWidgets.libdirs = { }	
	wxWidgets.links = { "wxWidgets" }
	wxWidgets.defines = { '_LIB', 'WXUSINGDLL', 'WXMONOLITHIC' }

	defines { wxWidgets.defines }

	local c = configuration "windows"
		defines { "__WXMSW__" }

	configuration (c)
end

project "Editor.Native"

	uuid "69765483-81E7-471A-A252-01D16EA17413"
	kind "SharedLib"
	flags { editor_flags }
	defines { ""API_EDITOR_DLL", "API_EDITOR_DLL_EXPORT" }

	SetupNativeProjects()
	dependson { Engine.name }
	
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

	configuration "*"
	
	files
	{
		"Editor.Native.lua",
		"**.h",
		"**.cpp",
		"**.rc",
	}
	
	SetupWxWidgets()

	includedirs
	{
		incdir,
		srcdir,
		path.join(srcdir,"Editor","Editor.Native"),
		wxWidgets.includedirs
	}

	libdirs { wxWidgets.libdirs }
	links { wxWidgets.links, "Core", "Engine", "Graphics" }
