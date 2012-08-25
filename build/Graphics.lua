Graphics = {}
Graphics.name = "Graphics"
Graphics.defines = {}

project "Graphics"

	kind "StaticLib"	
	builddeps { Core.name, Resources.name }

	defines { Core.defines, Resources.defines }

	files
	{
		"Graphics.lua",
		"../inc/Graphics/**.h",
		"../src/Graphics/**.cpp",
		"../src/Graphics/Backends/*.h",
		"../src/Graphics/Backends/*.cpp",
	}
	
	vpaths
	{
		[""] = { "../src/", "../inc/" },
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
	}

	Graphics.libdirs =
	{
	}

	Graphics.links =
	{
		"opengl32",
	}
	
	Graphics.deps =
	{
		"GLEW",
	}
	
	deps(Graphics.deps)