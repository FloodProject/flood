project "zziplib"

  SetupNativeDependencyProject()
  
  kind "StaticLib"
  language "C"
  files { "src/*.c" }
  includedirs { "include", "../zlib/include" }