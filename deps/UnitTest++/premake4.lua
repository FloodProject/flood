project "UnitTest++"
		
	local version = "1.4.0"
	local repo = "https://unittest-cpp.svn.sourceforge.net/svnroot/unittest-cpp"
	local license = "MIT"
		
  kind "StaticLib"
  files { "src/*.cpp" }
  includedirs { "include" }
  
  if os.is("Windows") then
    files { "src/Win32/*.cpp" }
  else
    files { "src/Posix/*.cpp" }
  end
