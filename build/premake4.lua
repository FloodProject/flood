-- vapor Build Settings (Premake)
-- Written by triton (2008-2010)

solution "vapor"

	configurations { 
		"Debug", 
		"Release" 
	}
	
	buildoptions { "-std=c++0x" }

	configuration "Debug"
	
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
	
		defines { "NDEBUG" }
		flags { "Optimize" }

	location ( _ACTION )
	objdir 	( _ACTION .. "/obj" )
	
	project "vaporCore"
	
		kind "StaticLib"
		language "C++"	
	
		-- Source files
		files {
			"../inc/vapor/core/*.h",
			"../src/core/**.h",
			"../src/core/**.cpp",
		}

		-- Include directories
		includedirs {
			"../inc",
			"../dep/jsoncpp/include",
			"../dep/boost_1_39_0",
			"../dep/fd_delegate/include",
		}	

	project "vaporEngine"
	
		kind "StaticLib"
		language "C++"

		location ( _ACTION )
		targetdir ( _ACTION .. "/lib" )

		-- Source files
		files {
			"../inc/Engine/**.h",
			"../src/Engine/**.h",
			"../src/Engine/**.cpp",
		}
		
		excludes { "Body.*", "Physics.*" }

		-- Include directories
		includedirs {
			"../inc",
			"../dep/jsoncpp/include",
			"../dep/boost_1_39_0",
			"../dep/fd_delegate/include",
			"../dep/freetype/include",
			"../dep/glew/include",
			"../dep/libogg-1.1.3/include",
			"../dep/libvorbis-1.2.0/include",
			"../dep/lua/include",
			"../dep/luabind/include",
			"../dep/OpenAL 1.1 SDK/include",
			"../dep/physfs-2.0.0/include",
			"../dep/SFML/include",
		}

		-- include only the Visual Leak Detector on VS builds
		configuration "vs*"
		
			includedirs {
				"../dep/vld/include",
			}

		-- Pre-compiled headers
		pchheader "vapor/PCH.h"
		pchsource "../src/Engine/PCH.cpp"
