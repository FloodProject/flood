Graphics = {}

project "Graphics"

	kind "StaticLib"
	flags { common_flags }
	
	builddeps { "Core", "Resources" }

	files
	{
		"Graphics.lua",
		"../inc/Graphics/**.h",
		"../src/Graphics/**.cpp",
	}
	
	vpaths
	{
		[""] = { "../../src/Graphics/", "../../inc/Graphics/" },
		["Buffers"] = { "*Buffer*" },
		["Managers"] = { "*Manager*" },
		["Capabilities"] = { "RenderCapabilities*" },
		["Contexts"] = { "RenderContext*" },
		["Queues"] = { "RenderQueue*" },
		["Textures"] = { "*Texture*" },
		["Shaders"] = { "*Shader*", "*Program*" },
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

	links
	{
		"opengl32",
		"glewd",
	}