project "OggVorbis"

	SetupNativeDependencyProject()
		
	local version = { "1.3.0", "1.3.3" }
	local repo = { "http://svn.xiph.org/trunk/ogg/",
    "http://svn.xiph.org/trunk/vorbis/" }
    
	local license = "BSD"
	
  kind "StaticLib"
  files { "src/Ogg/**.c" }
  files { "src/Vorbis/**.c" }
  includedirs { "include" }