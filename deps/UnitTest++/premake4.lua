project "UnitTest++"

	SetupNativeDependencyProject()
		
	local version = "1.4.0"
	local repo = "https://unittest-cpp.svn.sourceforge.net/svnroot/unittest-cpp"
	local license = "MIT"
		
  kind "StaticLib"
  files { "src/*.cpp" }
  includedirs { "include" }
  
  configuration "windows"
    files { "src/Win32/*.cpp" }

  configuration "not windows"
    files { "src/Posix/*.cpp" }

  configuration "not vs*"
    removeflags { "NoExceptions" }