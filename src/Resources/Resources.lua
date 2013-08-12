Resources = {}
Resources.name = "Resources"
Resources.shared = true
Resources.defines = {}

project "Resources"

	uuid "8848C478-A23F-4B45-8580-FB7BFDE58F9C"

	if Resources.shared then
		kind "SharedLib"
		table.insert(Resources.defines, "API_RESOURCE_DLL")
		defines { "API_RESOURCE_DLL_EXPORT" }
	else
		kind "StaticLib"
	end
	
	SetupNativeProjects()

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