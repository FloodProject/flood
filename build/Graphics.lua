Graphics = {}

project "Graphics"

	kind "StaticLib"
	flags { common_flags }
	
	builddeps { "Core", "Resources" }

	files
	{
		"Graphics.lua",
		"../inc/Graphics/*.h",
		"../src/Graphics/*.cpp",
		"../src/Graphics/Backends/*.h",
		"../src/Graphics/Backends/*.cpp",
	}
	
	vpaths
	{
		[""] = { "../../src/Graphics/", "../../inc/Graphics/" },
		["Buffers"] = { "*Buffer*" },
		["Managers"] = { "*Manager*" },
		["Capabilities"] = { "RenderCapabilities*" },
		["Contexts"] = { "RenderContext*" },
		["Queues"] = { "RenderQueue*" },
		["Textures"] = { "Texture*" },
		["Shaders"] = { "Shader*" },
		["Views"] = { "*RenderView*" },
	}

	includedirs
	{
		"../inc/",
		"../dep/glew/include",
	}

	Graphics.libdirs =
	{
		"../dep/glew/lib/vc10",
	}

	Graphics.links =
	{
		"opengl32",
	}

	Graphics.links.Debug =
	{
		"glewd",
	}

	Graphics.links.Release =
	{
		"glew",
	}