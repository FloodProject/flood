Engine = {}

project "Engine"

	kind "StaticLib"
	flags { common_flags }
	
	builddeps { "Core", "Resources", "Graphics" }

	pchheader "Engine/API.h"
	pchsource "../src/Engine/Engine.cpp"

	files
	{
		"Engine.lua",
		"../inc/Audio/**.h",
		"../src/Audio/**.cpp",
		"../inc/Engine/**.h",
		"../src/Engine/**.cpp",
		"../inc/Geometry/**.h",
		"../src/Geometry/**.cpp",
		"../inc/Scene/**.h",
		"../src/Scene/**.cpp",
		"../inc/Physics/**.h",
		"../src/Physics/**.cpp",
		"../inc/Controllers/**.h",
		"../src/Controllers/**.cpp",
		"../inc/Terrain/**.h",
		"../src/Terrain/**.cpp",
		"../inc/Input/**.h",
		"../src/Input/**.cpp",
		"../inc/Script/**.h",
		"../src/Script/**.cpp",
		"../inc/Paging/**.h",
		"../src/Paging/**.cpp",
		"../inc/GUI/**.h",
		"../src/GUI/**.cpp",
		"../inc/Window/**.h",
		"../src/Window/**.cpp",
		"../inc/Framework/**.h",
		"../src/Framework/**.cpp",
		"../interface/Bindings/*.i",
	}
	
	vpaths
	{
		[""] = { "../../src/", "../../inc/", "../../src/Engine/", "../../inc/Engine/" },
		["Audio"] = { "Audio*" },
		["Controllers"] = "Controllers/*",
		["Controllers/Camera"] = { "*Controller*" },
		["Framework"] = { "Framework" },
		["Geometry"] = { "*Geometry*" },
		["Graphics"] = { "Graphics/*" },
		["GUI"] = { "GUI/*" },
		["Input"] = "Input*",
		["Paging"] = "Paging*",
		["Physics"] = "Physics*",
		["Scene"] = "Scene*",
		["Scripts"] = { "Script*" },
		["Terrain"] = { "Terrain*" },
		["Window"] = { "Window*" },
	}

	configuration "not windows"
		excludes { "../*Win32*" }

	configuration "windows"
		vpaths { ["Window/Win32"] = { "**/Win32/WindowWin32*" } }

	configuration {}

	includedirs
	{
		"../inc/",
		"../interface/Bindings",
		"../dep/jansson/include",
		"../dep/freetype/include",
		"../dep/glew/include",
		"../dep/openal/include",
		"../dep/sfml/include",
		"../dep/Bullet/include",
		"../dep/sfml/",
	}

	Engine.libdirs =
	{
		"../dep/openal/lib/Win32",
		"../dep/Bullet/lib/",
		"../dep/glew/lib/vc10",
		"../dep/freetype/objs",
	}

	Engine.links =
	{
		"opengl32",
	}

	Engine.links.Debug =
	{
		"glewd",
		"openal_d",
		"BulletCollision_debug", "BulletDynamics_debug", "LinearMath_debug"
	}

	Engine.links.Release =
	{
		"glew",
		"openal",
		"BulletCollision", "BulletDynamics", "LinearMath"
	}
	
	configuration {}
		defines { "AL_LIBTYPE_STATIC" }