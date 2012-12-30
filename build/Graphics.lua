Graphics = {}
Graphics.name = "Graphics"
Graphics.shared = true
Graphics.defines = {}

project "Graphics"

	if Graphics.shared then
		kind "SharedLib"
		table.insert(Graphics.defines, "API_GRAPHICS_DLL")
		defines { "API_GRAPHICS_DLL_EXPORT" }
	else
		kind "StaticLib"
	end

	builddeps { Core.name, Resources.name }

	defines { Core.defines, Resources.defines, Graphics.defines }

	files
	{
		"Graphics.lua",
		"../inc/Graphics/*.h",
		"../inc/Graphics/Resources/*.h",
		"../src/Graphics/*.cpp",
		"../src/Graphics/Backends/*.h",
		"../src/Graphics/Backends/*.cpp",
		"../src/Graphics/Resources/*.h",
		"../src/Graphics/Resources/*.cpp",		
	}
	
	vpaths
	{
		["*"] = { "../src/Graphics/", "../inc/Graphics/" },
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

	links { Core.name, Resources.name, Graphics.links }
	
	Graphics.deps =
	{
		"GLEW",
	}
	
	deps(Graphics.deps)