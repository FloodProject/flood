Engine = {}
Engine.name = "Engine"
Engine.shared = true
Engine.defines = {}

project "Engine"

	if Engine.shared then
		kind "SharedLib"
		table.insert(Engine.defines, "API_ENGINE_DLL")
		defines { Engine.defines, "API_ENGINE_DLL_EXPORT" }
	else
		kind "StaticLib"
	end
	
	SetupNativeProjects()

	table.insert(Engine.defines, 
	{
	 	Core.defines,
	 	Resources.defines,
	 	Graphics.defines,
	})

	defines { Engine.defines }

	builddeps { Core.name, Resources.name, Graphics.name }

	pchheader "Engine/API.h"
	pchsource "../src/Engine/Engine.cpp"

	defines { "AL_LIBTYPE_STATIC" }

	files
	{
		"Engine.lua",
		"../inc/Engine/**.h",
		"../src/Engine/**.cpp",
	}
	
	vpaths
	{
		["*"] = { "../src/Engine/", "../inc/Engine/" },
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
		Resources.name,
		Graphics.name,
	}

	configuration "windows"
		links { "winmm", "ws2_32" }

	Engine.libdirs =
	{
		"../bin/",
		Graphics.libdirs
	}	
	
	if Core.shared == false then
		deps { Core.deps }
	end

	deps { Engine.deps  }
	links { Engine.links }
