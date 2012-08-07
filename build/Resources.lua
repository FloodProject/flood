Resources = {}

project "Resources"

	kind "StaticLib"
	builddeps { "Core" }
	
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
		[""] = { "../../src/Resources/", "../../inc/Resources/" },
		["Animation"] = { "Animation*", "Attachment*", "Bone*", "Skeleton*" },
		["Database"] = "ResourceDatabase*",
		["Fonts"] = { "*Font*", "TTF*" },
		["Images"] = { "Image*" },
		["Images/STB"] = { "STB*", "stb_image*" },
		["Images/PNG"] = { "lodepng*", "picopng*", "PNG_Loader*" },
		["Indexer"] = "ResourceIndexer*",
		["Materials"] = "Material*",
		["Meshes"] = "Mesh*",
		["Scripts"] = { "Script*", "LuaLoader*" },
		["Shaders"] = "Shader*",
		["Sounds"] = { "Sound*", "ResourceSound*" },
		["Sounds/OGG"] = "OGG*",
		["Text"] = "Text*",
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
		"Lode",
		"OggVorbis",
		"SeanBarrett"
	}
	
	deps(Resources.deps)