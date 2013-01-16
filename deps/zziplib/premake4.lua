project "zziplib"

  SetupNativeDependencyProject()
  
  kind "StaticLib"
  files { "src/*.c" }
  includedirs { "include", "../zlib/include" }		