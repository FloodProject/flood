project "zziplib"

  SetupDependencyProject()
  
  kind "StaticLib"
  files { "src/*.c" }
  includedirs { "include", "../zlib/include" }		