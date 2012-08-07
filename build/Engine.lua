Engine = {}

project "Engine"

	kind "StaticLib"
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
		"../interface/Bindings",
	}
	
	Engine.deps =
	{
		"Bullet",
		"OpenAL",
		"GLEW",
	}
	
	deps { Core.deps, Engine.deps }

	Engine.libdirs =
	{

	}

	Engine.links =
	{
		"opengl32",
	}

	configuration {}
		defines { "AL_LIBTYPE_STATIC" }