project "FastLZ"

	SetupNativeDependencyProject()
		
	local version = "0.1.0"
	local repo = "http://fastlz.googlecode.com/svn/trunk/"
	local license = "MIT"
	
  kind "StaticLib"
  files { "src/*.c" }
  includedirs { "include" }
