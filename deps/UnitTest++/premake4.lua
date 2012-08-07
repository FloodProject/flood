project "UnitTest++"
		
	local version = "1.4.0"
	local repo = "https://unittest-cpp.svn.sourceforge.net/svnroot/unittest-cpp"
	local license = "MIT"
		
  kind "StaticLib"
  files { "src/*.cpp" }
  includedirs { "include" }
  
  configuration "Windows"
    files { "src/Win32/*.cpp" }
    
  configuration "not Windows"
    files { "src/Posix/*.cpp" }
