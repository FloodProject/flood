-- vapor3D Build Script

local action = _ACTION or ""

solution "vapor"

	configurations { 
		"Debug", 
		"Release" 
	}
	
	configuration "not macosx"
	
		buildoptions "-std=c++0x"
	
	configuration "gcc"
	
		buildoptions "-Wno-invalid-offsetof"

	configuration "Debug"
	
		defines { "DEBUG" }
		flags { "Symbols" }

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
		
		files {
			"../src/Core/**.h",
			"../src/Core/**.cpp",
		}

		includedirs {
			"../src",
			"../inc/vapor",
			"../dep/jsoncpp/include",
			"../dep/curl/include",
			"../dep/zeromq/include",
			"../dep/physfs/",
		}
		
	project "Resources"
	
		kind "StaticLib"
		location (action)
		objdir (action .. "/obj")
		targetdir (action .. "/lib")
		
		files {
			"../src/Resources/**.h",
			"../src/Resources/**.cpp",
		}	
	
		includedirs {
			"../src",
			"../inc/vapor",
			"../src/Resources/",
			"../dep/stb/",
			"../dep/ogg/include",
			"../dep/vorbis/include",
		}

	project "Engine"
	
		kind "StaticLib"
		location (action)
		objdir (action .. "/obj")
		targetdir (action .. "/lib")

		files {
			"../src/Engine/**.h",
			"../src/Engine/**.cpp",
			"../dep/glew/src/glew.c"
		}

		includedirs {
			"../src",
			"../inc/",
			"../inc/vapor",
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