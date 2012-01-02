action = _ACTION or ""
common_flags = { "Unicode", "Symbols", "NoExceptions", "NoRTTI" }

solution "zziplib"

	configurations
	{ 
		"Debug",
		"Release"
	}
	
	language "C"
	
	location (action)
	objdir (action .. "/obj")
	targetdir (action .. "/lib")	
	
	-- Build configuration options
	
	configuration "Debug"
		defines { "DEBUG" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }	

	-- Compiler-specific options
	
	configuration "vs*"
		buildoptions { common_msvc_copts }	
	
	-- OS-specific options
	
	configuration "Windows"
		defines { "WIN32", "_WINDOWS" }
		
	configuration "*"

	project "zziplib"
		
		kind "StaticLib"
		targetdir ("lib/" .. action)
		flags { common_flags }
		
		files { "src/*.c" }
		includedirs { "include", "../zlib/include" }		
		
		configuration "Debug"
			targetsuffix "_d"

