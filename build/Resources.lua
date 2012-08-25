Resources = {}
Resources.name = "Resources"
Resources.defines = {}

project "Resources"

	kind "StaticLib"
	builddeps { "Core" }
	
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
		[""] = { "../src/", "../inc/" },
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
	}

	Resources.deps =
	{
	}
	
	deps(Resources.deps)