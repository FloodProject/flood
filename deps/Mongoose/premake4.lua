project "Mongoose"

	SetupNativeDependencyProject()
	DoNotBuildInNativeClient()

	local version = "3.0"
	local repo = "hg:https://code.google.com/p/mongoose"
	local license = "MIT"
	
  kind "StaticLib"
  files { "*.cpp" }
  includedirs { "include" }