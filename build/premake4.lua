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
	
	project "Core"
	
		kind "StaticLib"
		language "C++"	
	
		-- Source files
		files {
			"../inc/vapor/*.h",
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

	project "Engine"
	
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

		-- Include directories
		includedirs {
			"../inc",
			"../dep/boost",
			"../dep/jsoncpp/include",
			"../dep/fd_delegate/include",
			"../dep/freetype/include",
			"../dep/glew/include",
			"../dep/ogg/include",
			"../dep/vorbis/include",
			"../dep/lua/include",
			"../dep/openal/include",
			"../dep/physfs/include",
			"../dep/sfml/include",
		}

		-- include only the Visual Leak Detector on VS builds
		configuration "vs*"
		
			includedirs {
				"../dep/vld/include",
			}

		-- Pre-compiled headers
		pchheader "vapor/PCH.h"
		pchsource "../src/Engine/PCH.cpp"
