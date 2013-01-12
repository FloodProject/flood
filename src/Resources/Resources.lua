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
	
	SetupNativeProjects()

	builddeps { Core.name }
	
	defines { Core.defines, Resources.defines }

	pchheader "Resources/API.h"
	pchsource "ResourceManager.cpp"
	
	files
	{
		"Resources.lua",
		path.join(incdir,"Resources/**.h"),
		"**.cpp",
	}
	
	vpaths
	{
		["*"] = { ".", path.join(incdir,"Resources") },
	}	

	includedirs
	{
		incdir,
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