-- vapor3D Build Script

local action = _ACTION or ""
local common_flags = { "Unicode", "NoExceptions", "NoRTTI", "Symbols" }
local common_msvc_copts = { "/wd4190", "/wd4530" }

solution "vapor"

	configurations { 
		"Debug",
		"Release"
	}
	
	configuration { "not macosx", "gcc" }
	
		buildoptions "-std=gnu++0x"
		
	configuration "vs2010"
	
		buildoptions { common_msvc_copts }
	
	configuration "gcc"
	
		buildoptions "-Wno-invalid-offsetof"
		
	configuration "Debug"
	
		defines { "DEBUG" }

	configuration "Release"
	
		defines { "NDEBUG" }
		flags { "Optimize" }

	location (action)
	objdir (action .. "/obj")
	targetdir (action .. "/lib")
	
	language "C++"
	
	project "Core"
	
		kind "StaticLib"
		location (action)
		objdir (action .. "/obj")
		targetdir (action .. "/lib")
		
		flags { common_flags }
		
		files {
			"../inc/Core/**.h",
			"../src/Core/**.cpp",
			"../inc/Math/**.h",
			"../src/Math/**.cpp",
			"../dep/FastLZ/*"
		}
		
		vpaths {
			[""] = { "**src/Core*", "**inc/Core*" },
			["Archive"] = "**/Archive*",
			["Compression"] = "**/fastlz*",
			["Concurrency"] = "**/Concurren*",
			["Cryptography"] = "**/Cipher*",
			["Debug"] = "**/StackWalker*",
			["Events"] = { "**/Delegate*", "**/Event*" },
			["Extensions"] = "**/Plugin*",
			["Libraries"] = "**/DynamicLibrary*",
			["Logs"] = "**/Log*",
			["Math"] = "**/Math/**",
			["Memory"] = { "**/Handle*", "**/Memory*", "**/Pointers*", "**/References*" },
			["Reflection"] = { "**/ClassWatcher*", "**/Object*", "**/Reflection*" },
			["Serialization"] = "**/Serialization*",
			["Serialization/Binary"] = "**/BinarySerializer*",
			["Serialization/JSON"] = "**/JsonSerializer*",
			["Streams"] = "**/Stream*",
			["Streams/File"] = "**/FileStream*",
			["Streams/Memory"] = "**/MemoryStream*",
			["Streams/Web"] = "**/WebStream*",
			["Streams/Watcher"] = "**/FileWatcher*",
			["Utilities"] = "**/Utilities*",
			["Utilities/String"] = { "**/String*", "**/Pattern*" },
			["Utilities/Profiler"] = "**/Profiler*",
			["Utilities/Timer"] = "**/Timer*",
			["Utilities/Telemetry"] = "**/Telemetry*",
		}
		
		includedirs {
			"../inc/",
			"../dep/vld/include",
			"../dep/jansson/include",
			"../dep/zziplib/include",
			"../dep/mongoose",
			"../dep/enet/include",
			"../dep/curl/include",
			"../dep/misc/include",
		}
		
	project "Resources"
	
		kind "StaticLib"
		location (action)
		objdir (action .. "/obj")
		targetdir (action .. "/lib")
		
		flags { common_flags }
		
		files {
			"../inc/Resources/**.h",
			"../src/Resources/**.cpp",
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

	project "Engine"
	
		kind "StaticLib"
		location (action)
		objdir (action .. "/obj")
		targetdir (action .. "/lib")
		
		flags { common_flags }

		files {
			"../inc/Audio/**.h",
			"../src/Audio/**.cpp",
			"../inc/Engine/**.h",
			"../src/Engine/**.h",
			"../src/Engine/**.cpp",
			"../dep/glew/src/glew.c"
		}
		
		vpaths {
			[""] = { "**src/Engine*", "**inc/Engine*" },
			["Audio"] = { "**/Audio*" },
			["Controllers"] = "**/*Controller*",
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
			"../src",
			"../inc/",
			"../dep/freetype/include",
			"../dep/glew/include",
			"../dep/lua/include",
			"../dep/openal/include",
			"../dep/sfml/include",
			"../dep/bullet/include",
			"../dep/sfml/",
		}

		-- Pre-compiled headers
		--pchheader "vapor/PCH.h"
		--pchsource "../src/Engine/PCH.cpp"

	project "Example"
	
		kind "WindowedApp"
		location (action)
		objdir (action .. "/obj")
		targetdir ("../bin")
		
		flags { common_flags }
		
		files {
			"../src/Examples/**.h",
			"../src/Examples/**.cpp",
			"../src/Framework/**.cpp",
		}

		includedirs {
			"../src",
			"../inc/",
			"../inc/vapor",
			"../dep/freetype/include",
			"../dep/glew/include",
			"../dep/lua/include",
			"../dep/openal/include",
			"../dep/bullet/include"
		}
		
		libdirs {
			path.join(action, "lib"),
			"../dep/physfs/",
			"../dep/sfml/"
		}
		
		links {
			"Core", "Engine", "Resources",
			"physfs", "sfml-window.framework",
			"OpenAL.framework", "OpenGL.framework"
		}