project "stb_image"

	SetupNativeDependencyProject()
		
	local version = "1.33"
	local license = "Public Domain"
	local archive = "http://nothings.org/stb_vorbis/stb_vorbis.c"
	
  kind "StaticLib"
  language "C"
  files { "stb_image.c" }

project "stb_vorbis"

	SetupNativeDependencyProject()
		
	local version = "0.99996"
	local license = "Public Domain"
	local archive = "http://nothings.org/stb_image.c"
	
  kind "StaticLib"
  language "C"
  files { "stb_vorbis.c" }

 project "stb_truetype"
 
 	SetupNativeDependencyProject()
		
	local version = "0.6"
	local license = "Public Domain"
	local archive = "http://nothings.org/stb/stb_truetype.h"
	
  kind "StaticLib"
  language "C"
  files { "stb_truetype.c" }