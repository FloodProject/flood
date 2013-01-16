project "AnyOption"

  SetupNativeDependencyProject()
  
  kind "StaticLib"
  files { "src/*.cpp" }
  includedirs { "include" }