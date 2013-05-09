project "zlib"

	SetupNativeDependencyProject()
		
	local version = "1.2.7"
	local repo = "https://github.com/madler/zlib.git"
	local license = "zlib"
	
  kind "StaticLib"
  language "C"
  files { "src/*.c" }
  includedirs { "include" }
