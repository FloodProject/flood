Resources = {}
Resources.name = "Resources"
Resources.shared = true
Resources.defines = {}

project "Resources"

	if Resources.shared then
		kind "SharedLib"
		table.insert(Resources.defines, "API_RESOURCE_DLL")
		defines { "API_RESOURCE_DLL_EXPORT" }
	else
		kind "StaticLib"
	end

	builddeps { Core.name }
	
	defines { Core.defines, Resources.defines }

	pchheader "Resources/API.h"
	pchsource "../src/Resources/ResourceManager.cpp"
	
	files
	{
		"Resources.lua",
		"../inc/Resources/**.h",
		"../src/Resources/**.cpp",
	}
	
	vpaths
	{
		["*"] = { "../src/Resources/", "../inc/Resources/" },
	}	

	includedirs
	{
		"../inc/",
	}

	Resources.libdirs =
	{
	}
	
	Resources.links =
	{
		Core.name
	}

	links { Resources.links }

	Resources.deps =
	{
	}
	
	deps(Resources.deps)