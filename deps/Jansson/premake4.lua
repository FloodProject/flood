project "Jansson"
		
	local version = "2.0-fork"
	local repo = "https://github.com/tritao/jansson.git"
	local license = "MIT"
	
  kind "StaticLib"
  files { "src/*.c" }
  includedirs { "include" }