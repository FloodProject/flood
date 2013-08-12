Engine = {}
Engine.name = "Engine"
Engine.shared = true
Engine.defines = {}

project "Engine"

	uuid "3227B553-7C5A-4944-A097-B68AE248DDEE"

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

	pchheader "Engine/API.h"
	pchsource "Engine.cpp"

	defines { "AL_LIBTYPE_STATIC" }

	files
	{
		"Engine.lua",
		path.join(incdir,"Engine/**.h"),
		"**.cpp",
	}
	
	vpaths
	{
		["*"] = { ".", path.join(incdir,"Engine") },
	}

	configuration {}

	includedirs
	{
		incdir,
		path.join(depsdir,"SeanBarrett"),
		path.join(depsdir,"Jansson/include"),
	}
	
	Engine.deps =
	{
		"Bullet",
		"OpenAL",
		"OggVorbis",
		"stb_image",
		Graphics.deps,
        "stb_truetype",
        "FreeType",
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
		bindir,
		Graphics.libdirs
	}	
	
	if Core.shared == false then
		deps { Core.deps }
	end

	deps { Engine.deps  }
	links { Engine.links }
