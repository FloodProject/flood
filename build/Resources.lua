Resources = {}

project "Resources"

	kind "StaticLib"
	location (action)
	objdir (action .. "/obj")
	targetdir (action .. "/lib")

	builddeps { "Core" }
	
	flags { common_flags }

	pchheader "Resources/API.h"
	pchsource "../src/Resources/ResourceManager.cpp"
	
	files {
		"Resources.lua",
		"../inc/Resources/**.h",
		"../src/Resources/**.cpp",
		"../dep/picopng/lode*.cpp",
		--"../dep/stb/stb_image*.cpp",
	}
	
	vpaths {
		[""] = { "**src/Core*", "**inc/Core*" },
		["Animation"] = { "**/Animation*", "**/Attachment*", "**/Bone*", "**/Skeleton*" },
		["Database"] = "**/ResourceDatabase*",
		["Fonts"] = { "**/Font*", "**/TTF*" },
		["Images"] = { "**/Image*" },
		["Images/STB"] = { "**/STB*", "**/stb_image*" },
		["Images/PNG"] = { "**/lodepng*", "**/picopng*", "**/PNG_Loader*" },
		["Indexer"] = "**/ResourceIndexer*",
		["Materials"] = "**/Material*",
		["Meshes"] = "**/Mesh*",
		["Scripts"] = { "**/Script*", "**/LuaLoader*" },
		["Shaders"] = "**/Shader*",
		["Images/GLSL"] = { "**/GLSL*" },
		["Sounds"] = { "**/Sound*", "**/ResourceSound*" },
		["Sounds/OGG"] = "**/OGG*",
		["Text"] = "**/Text*",
	}

	includedirs {
		"../inc/",
		"../dep/stb/",
		"../dep/picopng/",
		"../dep/ogg/include",
		"../dep/vorbis/include",
	}

	Resources.libdirs = {
		"../dep/png/lib/",
		"../dep/ogg/lib",
		"../dep/vorbis/lib",
	}
	
	links {
		"pngd",
		"ogg_static_d",
		"vorbis_static_d",
		"vorbisfile_static_d"
	}