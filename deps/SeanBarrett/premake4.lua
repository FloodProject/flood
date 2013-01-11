project "stb_image"

	SetupDependencyProject()
		
	local version = "1.33"
	local license = "Public Domain"
	local archive = "http://nothings.org/stb_vorbis/stb_vorbis.c"
	
  kind "StaticLib"
  files { "stb_image.c" }

project "stb_vorbis"

	SetupDependencyProject()
		
	local version = "0.99996"
	local license = "Public Domain"
	local archive = "http://nothings.org/stb_image.c"
	
  kind "StaticLib"
  files { "stb_vorbis.c" }

 project "stb_truetype"
 
 	SetupDependencyProject()
		
	local version = "0.6"
	local license = "Public Domain"
	local archive = "http://nothings.org/stb/stb_truetype.h"
	
  kind "StaticLib"
  files { "stb_truetype.c" }