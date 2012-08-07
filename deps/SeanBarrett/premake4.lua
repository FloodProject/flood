project "STBImage"
		
	local version = "1.33"
	local license = "Public Domain"
	local archive = "http://nothings.org/stb_image.c"
	
  kind "StaticLib"
  files { "stb_image.c" }
 