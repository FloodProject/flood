project "zziplib"
  
  kind "StaticLib"
  files { "src/*.c" }
  includedirs { "include", "../zlib/include" }		