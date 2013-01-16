project "GLEW"

	SetupNativeDependencyProject()
		
	local version = "1.9.0"
	local repo = "git://glew.git.sourceforge.net/gitroot/glew/glew"
	local license = "MIT"
	
  kind "StaticLib"
  files { "src/*.c" }
  includedirs { "include" }
  defines { "GLEW_BUILD", "GLEW_STATIC" }