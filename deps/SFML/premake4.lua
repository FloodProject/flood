project "SFML"
		
	local version = "2.0"
	local repo = "https://github.com/LaurentGomila/SFML.git"
	local license = "zlib/png"
	
  kind "StaticLib"
  
  files
  {
  -- Headers
  	"include/*.hpp",

  -- Source files
  	"src/SFML/Graphics/*.cpp",	"src/SFML/Graphics/*.hpp",
  	"src/SFML/System/*.cpp",	"src/SFML/System/*.hpp",
  	"src/SFML/Window/*.cpp", 	"src/SFML/System/*.hpp",
  }
  
  includedirs { "include", "src", "extlibs/headers", "extlibs/headers/jpeg" }
  defines { "SFML_STATIC" }

  configuration "Windows"
  	files
  	{
  		"src/SFML/System/Win32/*.cpp", "src/SFML/System/Win32/*.hpp",
  		"src/SFML/Window/Win32/*.cpp", "src/SFML/Window/Win32/*.hpp",
  	}
	
	libdirs { "extlibs/libs-msvc/x86" }

	links { "freetype", "glew", "jpeg" }
