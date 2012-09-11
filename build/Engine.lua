Engine = {}
Engine.name = "Engine"
Engine.isShared = true
Engine.defines = {}

project "Engine"

	if Engine.isShared then
		kind "SharedLib"
		table.insert(Engine.defines, "API_ENGINE_DLL")
		defines { Engine.defines, "API_ENGINE_DLL_EXPORT" }
	else
		kind "StaticLib"
	end

	defines { Core.defines }

	builddeps { Core.name, Resources.name, Graphics.name }

	pchheader "Engine/API.h"
	pchsource "../src/Engine/Engine.cpp"

	defines { "AL_LIBTYPE_STATIC", "SFML_WINDOW_EXPORTS" }

	files
	{
		"Engine.lua",
		"../inc/Engine/**.h",
		"../src/Engine/**.cpp",
		"../src/Platforms/SFML/*.h",
		"../src/Platforms/SFML/*.cpp",
	}
	
	vpaths
	{
		[""] = { "../src/", "../inc/" },
	}

	configuration "not windows"
		excludes { "../*Win32*" }

	configuration "windows"
		vpaths { ["Window/Win32"] = { "**/Win32/WindowWin32*" } }

	configuration {}

	includedirs
	{
		"../inc/",
		"../deps/SeanBarrett",
		"../deps/Jansson/include",
		"../deps/SFML/include/",
	}
	
	Engine.deps =
	{
		"Bullet",
		"OpenAL",
		"OggVorbis",
		"stb_image",
		Graphics.deps
		--"LodePNG",
	}

	Engine.links =
	{
		Core.name,
		Graphics.name,
		Resources.name,
		Graphics.links
	}

	configuration "windows"
		links { "winmm", "ws2_32" }

	Engine.libdirs =
	{
		"../bin/",
		Graphics.libdirs
	}	
	
	if Core.isShared == false then
		deps { Core.deps }
	else
		SetupLibLinks(Core.name)
	end

	deps { Engine.deps  }
	links { Engine.links }
	libdirs { Engine.libdirs }

	configuration "Debug"
		links { "SFML_d" }
	configuration "Release"
		links { "SFML" }