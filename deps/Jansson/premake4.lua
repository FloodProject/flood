project "Jansson"

	SetupNativeDependencyProject()
		
	local version = "2.0-fork"
	local repo = "https://github.com/tritao/jansson.git"
	local license = "MIT"
	
  kind "StaticLib"
  language "C"
  files { "src/*.c" }
  includedirs { "include" }